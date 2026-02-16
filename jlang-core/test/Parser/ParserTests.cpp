#include <gtest/gtest.h>

#include <jlang/Parser/Parser.h>
#include <jlang/Scanner/Scanner.h>

using namespace jlang;

// Helper: source -> parsed AST nodes
static std::vector<std::shared_ptr<AstNode>> Parse(const std::string &source)
{
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();
    Parser parser(tokens);
    return parser.Parse();
}

// Helper: parse a single top-level declaration
static std::shared_ptr<AstNode> ParseOne(const std::string &source)
{
    auto nodes = Parse(source);
    EXPECT_GE(nodes.size(), 1u);
    return nodes.empty() ? nullptr : nodes[0];
}

// Helper: parse the body of a function and return its block statement
static std::shared_ptr<BlockStatement> ParseFnBody(const std::string &bodySource)
{
    std::string source = "fn test() -> void { " + bodySource + " }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));
    EXPECT_NE(fn, nullptr);
    auto block = std::dynamic_pointer_cast<BlockStatement>(fn->body);
    EXPECT_NE(block, nullptr);
    return block;
}

// Helper: parse a single expression inside a function body
static std::shared_ptr<AstNode> ParseExpr(const std::string &exprSource)
{
    auto block = ParseFnBody(exprSource + ";");
    EXPECT_GE(block->statements.size(), 1u);
    auto exprStmt = std::dynamic_pointer_cast<ExprStatement>(block->statements[0]);
    EXPECT_NE(exprStmt, nullptr);
    return exprStmt ? exprStmt->expression : nullptr;
}

// ============================================================================
// Function declarations
// ============================================================================

TEST(ParserTest, ParsesEmptyFunction)
{
    std::string source = "fn main() -> void {}";
    auto node = ParseOne(source);
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(node);

    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "main");
    EXPECT_EQ(fn->params.size(), 0u);
    EXPECT_EQ(fn->returnType.name, "void");
    EXPECT_FALSE(fn->returnType.isPointer);
    EXPECT_TRUE(fn->typeParameters.empty());

    auto body = std::dynamic_pointer_cast<BlockStatement>(fn->body);
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->statements.size(), 0u);
}

TEST(ParserTest, ParsesFunctionWithParams)
{
    std::string source = "fn add(a: i32, b: i32) -> i32 { return a; }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "add");
    ASSERT_EQ(fn->params.size(), 2u);
    EXPECT_EQ(fn->params[0].name, "a");
    EXPECT_EQ(fn->params[0].type.name, "i32");
    EXPECT_EQ(fn->params[1].name, "b");
    EXPECT_EQ(fn->params[1].type.name, "i32");
    EXPECT_EQ(fn->returnType.name, "i32");
}

TEST(ParserTest, ParsesFunctionWithPointerParam)
{
    std::string source = "fn deref(p: i32*) -> i32 { return p; }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    ASSERT_EQ(fn->params.size(), 1u);
    EXPECT_EQ(fn->params[0].type.name, "i32");
    EXPECT_TRUE(fn->params[0].type.isPointer);
}

TEST(ParserTest, ParsesFunctionWithPointerReturnType)
{
    std::string source = "fn create() -> Node* { return null; }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->returnType.name, "Node");
    EXPECT_TRUE(fn->returnType.isPointer);
}

TEST(ParserTest, ParsesGenericFunction)
{
    std::string source = "fn identity<T>(x: T) -> T { return x; }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "identity");
    ASSERT_EQ(fn->typeParameters.size(), 1u);
    EXPECT_EQ(fn->typeParameters[0], "T");
    ASSERT_EQ(fn->params.size(), 1u);
    EXPECT_EQ(fn->params[0].type.name, "T");
    EXPECT_EQ(fn->returnType.name, "T");
}

TEST(ParserTest, ParsesGenericFunctionMultipleParams)
{
    std::string source = "fn pair<T, U>(a: T, b: U) -> void {}";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    ASSERT_EQ(fn->typeParameters.size(), 2u);
    EXPECT_EQ(fn->typeParameters[0], "T");
    EXPECT_EQ(fn->typeParameters[1], "U");
}

TEST(ParserTest, ParsesFunctionDefaultVoidReturn)
{
    std::string source = "fn noop() {}";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->returnType.name, "void");
}

TEST(ParserTest, ParsesFunctionNullablePointerReturn)
{
    std::string source = "fn find() -> Node*? { return null; }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));

    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->returnType.name, "Node");
    EXPECT_TRUE(fn->returnType.isPointer);
    EXPECT_TRUE(fn->returnType.isNullable);
}

// ============================================================================
// Struct declarations
// ============================================================================

TEST(ParserTest, ParsesEmptyStruct)
{
    std::string source = "struct Empty {}";
    auto node = ParseOne(source);
    auto s = std::dynamic_pointer_cast<StructDecl>(node);

    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->name, "Empty");
    EXPECT_EQ(s->fields.size(), 0u);
    EXPECT_TRUE(s->interfaceImplemented.empty());
    EXPECT_TRUE(s->typeParameters.empty());
}

TEST(ParserTest, ParsesStructWithFields)
{
    std::string source = "struct Point { x: f64; y: f64; }";
    auto s = std::dynamic_pointer_cast<StructDecl>(ParseOne(source));

    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->name, "Point");
    ASSERT_EQ(s->fields.size(), 2u);
    EXPECT_EQ(s->fields[0].name, "x");
    EXPECT_EQ(s->fields[0].type.name, "f64");
    EXPECT_EQ(s->fields[1].name, "y");
    EXPECT_EQ(s->fields[1].type.name, "f64");
}

TEST(ParserTest, ParsesStructFieldVisibility)
{
    std::string source = "struct Foo { Name: i32; value: i32; }";
    auto s = std::dynamic_pointer_cast<StructDecl>(ParseOne(source));

    ASSERT_NE(s, nullptr);
    ASSERT_EQ(s->fields.size(), 2u);
    EXPECT_TRUE(s->fields[0].isPublic);  // Uppercase = public
    EXPECT_FALSE(s->fields[1].isPublic); // lowercase = private
}

TEST(ParserTest, ParsesStructWithPointerField)
{
    std::string source = "struct Node { next: Node*; }";
    auto s = std::dynamic_pointer_cast<StructDecl>(ParseOne(source));

    ASSERT_NE(s, nullptr);
    ASSERT_EQ(s->fields.size(), 1u);
    EXPECT_EQ(s->fields[0].type.name, "Node");
    EXPECT_TRUE(s->fields[0].type.isPointer);
}

TEST(ParserTest, ParsesStructWithNullableField)
{
    std::string source = "struct Node { next: Node*?; }";
    auto s = std::dynamic_pointer_cast<StructDecl>(ParseOne(source));

    ASSERT_NE(s, nullptr);
    ASSERT_EQ(s->fields.size(), 1u);
    EXPECT_TRUE(s->fields[0].type.isPointer);
    EXPECT_TRUE(s->fields[0].type.isNullable);
}

TEST(ParserTest, ParsesStructWithInterface)
{
    std::string source = "struct Circle : Drawable { radius: f64; }";
    auto s = std::dynamic_pointer_cast<StructDecl>(ParseOne(source));

    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->name, "Circle");
    EXPECT_EQ(s->interfaceImplemented, "Drawable");
}

TEST(ParserTest, ParsesGenericStruct)
{
    std::string source = "struct Pair<T, U> { first: T; second: U; }";
    auto s = std::dynamic_pointer_cast<StructDecl>(ParseOne(source));

    ASSERT_NE(s, nullptr);
    ASSERT_EQ(s->typeParameters.size(), 2u);
    EXPECT_EQ(s->typeParameters[0], "T");
    EXPECT_EQ(s->typeParameters[1], "U");
    ASSERT_EQ(s->fields.size(), 2u);
    EXPECT_EQ(s->fields[0].type.name, "T");
    EXPECT_EQ(s->fields[1].type.name, "U");
}

// ============================================================================
// Interface declarations
// ============================================================================

TEST(ParserTest, ParsesInterface)
{
    std::string source = "interface Drawable { fn draw(x: i32) -> void; }";
    auto iface = std::dynamic_pointer_cast<InterfaceDecl>(ParseOne(source));

    ASSERT_NE(iface, nullptr);
    EXPECT_EQ(iface->name, "Drawable");
    ASSERT_EQ(iface->methods.size(), 1u);
    EXPECT_EQ(iface->methods[0].name, "draw");
    ASSERT_EQ(iface->methods[0].params.size(), 1u);
    EXPECT_EQ(iface->methods[0].params[0].name, "x");
    EXPECT_EQ(iface->methods[0].params[0].type.name, "i32");
    EXPECT_EQ(iface->methods[0].returnType.name, "void");
}

TEST(ParserTest, ParsesInterfaceMultipleMethods)
{
    std::string source = "interface Shape { fn area() -> f64; fn name() -> i32; }";
    auto iface = std::dynamic_pointer_cast<InterfaceDecl>(ParseOne(source));

    ASSERT_NE(iface, nullptr);
    ASSERT_EQ(iface->methods.size(), 2u);
    EXPECT_EQ(iface->methods[0].name, "area");
    EXPECT_EQ(iface->methods[1].name, "name");
}

// ============================================================================
// Variable declarations
// ============================================================================

TEST(ParserTest, ParsesVarDeclWithType)
{
    auto block = ParseFnBody("var x: i32 = 42;");
    ASSERT_EQ(block->statements.size(), 1u);

    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);
    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->name, "x");
    EXPECT_EQ(varDecl->varType.name, "i32");
    EXPECT_TRUE(varDecl->isMutable);
    ASSERT_NE(varDecl->initializer, nullptr);

    auto init = std::dynamic_pointer_cast<LiteralExpr>(varDecl->initializer);
    ASSERT_NE(init, nullptr);
    EXPECT_EQ(init->value, "42");
}

TEST(ParserTest, ParsesValImmutableDecl)
{
    auto block = ParseFnBody("val x: i32 = 10;");
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_FALSE(varDecl->isMutable);
}

TEST(ParserTest, ParsesVarDeclWithTypeInference)
{
    auto block = ParseFnBody("var x := 42;");
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->name, "x");
    EXPECT_TRUE(varDecl->varType.name.empty()); // inferred
    ASSERT_NE(varDecl->initializer, nullptr);
}

TEST(ParserTest, ParsesVarDeclWithPointerType)
{
    auto block = ParseFnBody("var p: Node* = null;");
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->varType.name, "Node");
    EXPECT_TRUE(varDecl->varType.isPointer);
}

TEST(ParserTest, ParsesVarDeclWithNullablePointer)
{
    auto block = ParseFnBody("var p: Node*? = null;");
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_TRUE(varDecl->varType.isPointer);
    EXPECT_TRUE(varDecl->varType.isNullable);
}

TEST(ParserTest, ParsesVarDeclWithArrayType)
{
    auto block = ParseFnBody("var arr: i32[10];");
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->varType.name, "i32");
    EXPECT_TRUE(varDecl->varType.isArray);
    EXPECT_EQ(varDecl->varType.arraySize, 10);
}

TEST(ParserTest, ParsesVarDeclWithGenericType)
{
    std::string source = "fn test() -> void { var r: Result<i32, char*>; }";
    auto fn = std::dynamic_pointer_cast<FunctionDecl>(ParseOne(source));
    auto block = std::dynamic_pointer_cast<BlockStatement>(fn->body);
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->varType.name, "Result");
    ASSERT_EQ(varDecl->varType.typeParameters.size(), 2u);
    EXPECT_EQ(varDecl->varType.typeParameters[0].name, "i32");
    EXPECT_EQ(varDecl->varType.typeParameters[1].name, "char");
    EXPECT_TRUE(varDecl->varType.typeParameters[1].isPointer);
}

TEST(ParserTest, ParsesVarDeclWithoutInitializer)
{
    auto block = ParseFnBody("var x: i32;");
    auto varDecl = std::dynamic_pointer_cast<VariableDecl>(block->statements[0]);

    ASSERT_NE(varDecl, nullptr);
    EXPECT_EQ(varDecl->name, "x");
    EXPECT_EQ(varDecl->initializer, nullptr);
}

// ============================================================================
// Statements
// ============================================================================

TEST(ParserTest, ParsesReturnWithValue)
{
    auto block = ParseFnBody("return 42;");
    auto ret = std::dynamic_pointer_cast<ReturnStatement>(block->statements[0]);

    ASSERT_NE(ret, nullptr);
    ASSERT_NE(ret->value, nullptr);
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(ret->value);
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, "42");
}

TEST(ParserTest, ParsesReturnVoid)
{
    auto block = ParseFnBody("return;");
    auto ret = std::dynamic_pointer_cast<ReturnStatement>(block->statements[0]);

    ASSERT_NE(ret, nullptr);
    EXPECT_EQ(ret->value, nullptr);
}

TEST(ParserTest, ParsesBreakStatement)
{
    auto block = ParseFnBody("break;");
    auto brk = std::dynamic_pointer_cast<BreakStatement>(block->statements[0]);
    ASSERT_NE(brk, nullptr);
}

TEST(ParserTest, ParsesContinueStatement)
{
    auto block = ParseFnBody("continue;");
    auto cont = std::dynamic_pointer_cast<ContinueStatement>(block->statements[0]);
    ASSERT_NE(cont, nullptr);
}

TEST(ParserTest, ParsesIfStatement)
{
    auto block = ParseFnBody("if (x) { return; }");
    auto ifStmt = std::dynamic_pointer_cast<IfStatement>(block->statements[0]);

    ASSERT_NE(ifStmt, nullptr);
    ASSERT_NE(ifStmt->condition, nullptr);
    ASSERT_NE(ifStmt->thenBranch, nullptr);
    EXPECT_EQ(ifStmt->elseBranch, nullptr);
}

TEST(ParserTest, ParsesIfElseStatement)
{
    auto block = ParseFnBody("if (x) { return; } else { return; }");
    auto ifStmt = std::dynamic_pointer_cast<IfStatement>(block->statements[0]);

    ASSERT_NE(ifStmt, nullptr);
    ASSERT_NE(ifStmt->thenBranch, nullptr);
    ASSERT_NE(ifStmt->elseBranch, nullptr);
}

TEST(ParserTest, ParsesIfElseIfChain)
{
    std::string src = "if (a) { return; } else if (b) { return; } else { return; }";
    auto block = ParseFnBody(src);
    auto ifStmt = std::dynamic_pointer_cast<IfStatement>(block->statements[0]);

    ASSERT_NE(ifStmt, nullptr);
    // else branch is another IfStatement
    auto elseIf = std::dynamic_pointer_cast<IfStatement>(ifStmt->elseBranch);
    ASSERT_NE(elseIf, nullptr);
    ASSERT_NE(elseIf->elseBranch, nullptr);
}

TEST(ParserTest, ParsesWhileStatement)
{
    auto block = ParseFnBody("while (x) { break; }");
    auto whileStmt = std::dynamic_pointer_cast<WhileStatement>(block->statements[0]);

    ASSERT_NE(whileStmt, nullptr);
    ASSERT_NE(whileStmt->condition, nullptr);
    ASSERT_NE(whileStmt->body, nullptr);
}

TEST(ParserTest, ParsesForStatement)
{
    std::string src = "for (var i: i32 = 0; i < 10; i++) {}";
    auto block = ParseFnBody(src);
    auto forStmt = std::dynamic_pointer_cast<ForStatement>(block->statements[0]);

    ASSERT_NE(forStmt, nullptr);
    ASSERT_NE(forStmt->init, nullptr);
    ASSERT_NE(forStmt->condition, nullptr);
    ASSERT_NE(forStmt->update, nullptr);
    ASSERT_NE(forStmt->body, nullptr);

    // init is a variable declaration
    auto initVar = std::dynamic_pointer_cast<VariableDecl>(forStmt->init);
    ASSERT_NE(initVar, nullptr);
    EXPECT_EQ(initVar->name, "i");
}

TEST(ParserTest, ParsesForStatementEmptyClauses)
{
    auto block = ParseFnBody("for (;;) { break; }");
    auto forStmt = std::dynamic_pointer_cast<ForStatement>(block->statements[0]);

    ASSERT_NE(forStmt, nullptr);
    EXPECT_EQ(forStmt->init, nullptr);
    EXPECT_EQ(forStmt->condition, nullptr);
    EXPECT_EQ(forStmt->update, nullptr);
}

TEST(ParserTest, ParsesForEachStatement)
{
    auto block = ParseFnBody("for elem in items {}");
    auto forEach = std::dynamic_pointer_cast<ForEachStatement>(block->statements[0]);

    ASSERT_NE(forEach, nullptr);
    EXPECT_EQ(forEach->elementName, "elem");
    ASSERT_NE(forEach->iterable, nullptr);
    auto iterVar = std::dynamic_pointer_cast<VarExpr>(forEach->iterable);
    ASSERT_NE(iterVar, nullptr);
    EXPECT_EQ(iterVar->name, "items");
}

TEST(ParserTest, ParsesNestedBlocks)
{
    auto block = ParseFnBody("{ var x: i32 = 1; { var y: i32 = 2; } }");
    ASSERT_EQ(block->statements.size(), 1u);

    auto inner = std::dynamic_pointer_cast<BlockStatement>(block->statements[0]);
    ASSERT_NE(inner, nullptr);
    ASSERT_EQ(inner->statements.size(), 2u);
}

// ============================================================================
// Literal expressions
// ============================================================================

TEST(ParserTest, ParsesIntegerLiteral)
{
    auto expr = ParseExpr("42");
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr);
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, "42");
}

TEST(ParserTest, ParsesFloatLiteral)
{
    auto expr = ParseExpr("3.14");
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr);
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, "3.14");
}

TEST(ParserTest, ParsesBoolLiterals)
{
    auto t = std::dynamic_pointer_cast<LiteralExpr>(ParseExpr("true"));
    auto f = std::dynamic_pointer_cast<LiteralExpr>(ParseExpr("false"));
    ASSERT_NE(t, nullptr);
    ASSERT_NE(f, nullptr);
    EXPECT_EQ(t->value, "true");
    EXPECT_EQ(f->value, "false");
}

TEST(ParserTest, ParsesNullLiteral)
{
    auto expr = ParseExpr("null");
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr);
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, "null");
}

TEST(ParserTest, ParsesStringLiteral)
{
    auto block = ParseFnBody("\"hello\";");
    auto exprStmt = std::dynamic_pointer_cast<ExprStatement>(block->statements[0]);
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(exprStmt->expression);
    ASSERT_NE(lit, nullptr);
    // Parser wraps string value in quotes
    EXPECT_EQ(lit->value, "\"hello\"");
}

TEST(ParserTest, ParsesCharLiteral)
{
    auto block = ParseFnBody("'a';");
    auto exprStmt = std::dynamic_pointer_cast<ExprStatement>(block->statements[0]);
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(exprStmt->expression);
    ASSERT_NE(lit, nullptr);
    // Parser wraps char value in single quotes
    EXPECT_EQ(lit->value, "'a'");
}

// ============================================================================
// Binary expressions and operator precedence
// ============================================================================

TEST(ParserTest, ParsesAddition)
{
    auto expr = ParseExpr("a + b");
    auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "+");

    auto left = std::dynamic_pointer_cast<VarExpr>(bin->left);
    auto right = std::dynamic_pointer_cast<VarExpr>(bin->right);
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->name, "a");
    EXPECT_EQ(right->name, "b");
}

TEST(ParserTest, ParsesMultiplicationHigherThanAddition)
{
    // a + b * c => a + (b * c)
    auto expr = ParseExpr("a + b * c");
    auto add = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(add, nullptr);
    EXPECT_EQ(add->op, "+");

    auto mul = std::dynamic_pointer_cast<BinaryExpr>(add->right);
    ASSERT_NE(mul, nullptr);
    EXPECT_EQ(mul->op, "*");
}

TEST(ParserTest, ParsesParenthesizedExpression)
{
    // (a + b) * c should be grouped expression, not cast
    auto expr = ParseExpr("(a + b) * c");
    auto mul = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(mul, nullptr);
    EXPECT_EQ(mul->op, "*");

    auto add = std::dynamic_pointer_cast<BinaryExpr>(mul->left);
    ASSERT_NE(add, nullptr);
    EXPECT_EQ(add->op, "+");
}

TEST(ParserTest, ParsesParenthesizedVariable)
{
    // (a) should be grouped expression, not cast to type "a"
    auto expr = ParseExpr("(a)");
    auto var = std::dynamic_pointer_cast<VarExpr>(expr);
    ASSERT_NE(var, nullptr);
    EXPECT_EQ(var->name, "a");
}

TEST(ParserTest, ParsesComparisonOperators)
{
    auto lt = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a < b"));
    auto gt = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a > b"));
    auto le = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a <= b"));
    auto ge = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a >= b"));
    auto eq = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a == b"));
    auto ne = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a != b"));

    ASSERT_NE(lt, nullptr);
    EXPECT_EQ(lt->op, "<");
    ASSERT_NE(gt, nullptr);
    EXPECT_EQ(gt->op, ">");
    ASSERT_NE(le, nullptr);
    EXPECT_EQ(le->op, "<=");
    ASSERT_NE(ge, nullptr);
    EXPECT_EQ(ge->op, ">=");
    ASSERT_NE(eq, nullptr);
    EXPECT_EQ(eq->op, "==");
    ASSERT_NE(ne, nullptr);
    EXPECT_EQ(ne->op, "!=");
}

TEST(ParserTest, ParsesLogicalOperators)
{
    // a && b || c => (a && b) || c
    auto expr = ParseExpr("a && b || c");
    auto orExpr = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(orExpr, nullptr);
    EXPECT_EQ(orExpr->op, "||");

    auto andExpr = std::dynamic_pointer_cast<BinaryExpr>(orExpr->left);
    ASSERT_NE(andExpr, nullptr);
    EXPECT_EQ(andExpr->op, "&&");
}

TEST(ParserTest, ParsesBitwiseOperators)
{
    auto ampersand = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a & b"));
    auto pipe = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a | b"));
    auto caret = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a ^ b"));

    ASSERT_NE(ampersand, nullptr);
    EXPECT_EQ(ampersand->op, "&");
    ASSERT_NE(pipe, nullptr);
    EXPECT_EQ(pipe->op, "|");
    ASSERT_NE(caret, nullptr);
    EXPECT_EQ(caret->op, "^");
}

TEST(ParserTest, ParsesShiftOperators)
{
    auto lsh = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a << b"));
    auto rsh = std::dynamic_pointer_cast<BinaryExpr>(ParseExpr("a >> b"));

    ASSERT_NE(lsh, nullptr);
    EXPECT_EQ(lsh->op, "<<");
    ASSERT_NE(rsh, nullptr);
    EXPECT_EQ(rsh->op, ">>");
}

TEST(ParserTest, ParsesBitwisePrecedence)
{
    // a | b & c => a | (b & c) since & binds tighter than |
    auto expr = ParseExpr("a | b & c");
    auto orExpr = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(orExpr, nullptr);
    EXPECT_EQ(orExpr->op, "|");

    auto andExpr = std::dynamic_pointer_cast<BinaryExpr>(orExpr->right);
    ASSERT_NE(andExpr, nullptr);
    EXPECT_EQ(andExpr->op, "&");
}

TEST(ParserTest, ParsesElvisOperator)
{
    auto expr = ParseExpr("a ?: b");
    auto elvis = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(elvis, nullptr);
    EXPECT_EQ(elvis->op, "?:");
}

TEST(ParserTest, ParsesModuloOperator)
{
    auto expr = ParseExpr("a % b");
    auto mod = std::dynamic_pointer_cast<BinaryExpr>(expr);
    ASSERT_NE(mod, nullptr);
    EXPECT_EQ(mod->op, "%");
}

// ============================================================================
// Unary expressions
// ============================================================================

TEST(ParserTest, ParsesUnaryNegation)
{
    auto expr = ParseExpr("-x");
    auto unary = std::dynamic_pointer_cast<UnaryExpr>(expr);
    ASSERT_NE(unary, nullptr);
    EXPECT_EQ(unary->op, "-");

    auto operand = std::dynamic_pointer_cast<VarExpr>(unary->operand);
    ASSERT_NE(operand, nullptr);
    EXPECT_EQ(operand->name, "x");
}

TEST(ParserTest, ParsesUnaryNot)
{
    auto expr = ParseExpr("!x");
    auto unary = std::dynamic_pointer_cast<UnaryExpr>(expr);
    ASSERT_NE(unary, nullptr);
    EXPECT_EQ(unary->op, "!");
}

TEST(ParserTest, ParsesUnaryBitwiseNot)
{
    auto expr = ParseExpr("~x");
    auto unary = std::dynamic_pointer_cast<UnaryExpr>(expr);
    ASSERT_NE(unary, nullptr);
    EXPECT_EQ(unary->op, "~");
}

TEST(ParserTest, ParsesPrefixIncrement)
{
    auto expr = ParseExpr("++x");
    auto prefix = std::dynamic_pointer_cast<PrefixExpr>(expr);
    ASSERT_NE(prefix, nullptr);
    EXPECT_EQ(prefix->op, "++");
}

TEST(ParserTest, ParsesPrefixDecrement)
{
    auto expr = ParseExpr("--x");
    auto prefix = std::dynamic_pointer_cast<PrefixExpr>(expr);
    ASSERT_NE(prefix, nullptr);
    EXPECT_EQ(prefix->op, "--");
}

TEST(ParserTest, ParsesPostfixIncrement)
{
    auto expr = ParseExpr("x++");
    auto postfix = std::dynamic_pointer_cast<PostfixExpr>(expr);
    ASSERT_NE(postfix, nullptr);
    EXPECT_EQ(postfix->op, "++");

    auto operand = std::dynamic_pointer_cast<VarExpr>(postfix->operand);
    ASSERT_NE(operand, nullptr);
    EXPECT_EQ(operand->name, "x");
}

TEST(ParserTest, ParsesPostfixDecrement)
{
    auto expr = ParseExpr("x--");
    auto postfix = std::dynamic_pointer_cast<PostfixExpr>(expr);
    ASSERT_NE(postfix, nullptr);
    EXPECT_EQ(postfix->op, "--");
}

// ============================================================================
// Assignment expressions
// ============================================================================

TEST(ParserTest, ParsesSimpleAssignment)
{
    auto expr = ParseExpr("x = 42");
    auto assign = std::dynamic_pointer_cast<AssignExpr>(expr);
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->name, "x");

    auto val = std::dynamic_pointer_cast<LiteralExpr>(assign->value);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(val->value, "42");
}

TEST(ParserTest, ParsesCompoundAssignmentPlusEqual)
{
    auto expr = ParseExpr("x += 1");
    auto assign = std::dynamic_pointer_cast<AssignExpr>(expr);
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->name, "x");

    // x += 1 is desugared to x = x + 1
    auto bin = std::dynamic_pointer_cast<BinaryExpr>(assign->value);
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "+");
}

TEST(ParserTest, ParsesCompoundAssignmentAllOps)
{
    auto check = [](const std::string &src, const std::string &expectedOp) {
        auto expr = ParseExpr(src);
        auto assign = std::dynamic_pointer_cast<AssignExpr>(expr);
        ASSERT_NE(assign, nullptr) << "Failed for: " << src;
        auto bin = std::dynamic_pointer_cast<BinaryExpr>(assign->value);
        ASSERT_NE(bin, nullptr) << "Failed for: " << src;
        EXPECT_EQ(bin->op, expectedOp) << "Failed for: " << src;
    };

    check("x -= 1", "-");
    check("x *= 2", "*");
    check("x /= 3", "/");
    check("x %= 4", "%");
    check("x &= 5", "&");
    check("x |= 6", "|");
    check("x ^= 7", "^");
    check("x <<= 8", "<<");
    check("x >>= 9", ">>");
}

TEST(ParserTest, ParsesIndexAssignment)
{
    auto expr = ParseExpr("arr[0] = 42");
    auto indexAssign = std::dynamic_pointer_cast<IndexAssignExpr>(expr);
    ASSERT_NE(indexAssign, nullptr);
    ASSERT_NE(indexAssign->value, nullptr);
}

TEST(ParserTest, ParsesMemberAssignment)
{
    auto expr = ParseExpr("obj.field = 42");
    auto memberAssign = std::dynamic_pointer_cast<MemberAssignExpr>(expr);
    ASSERT_NE(memberAssign, nullptr);
    EXPECT_EQ(memberAssign->memberName, "field");
}

// ============================================================================
// Call expressions
// ============================================================================

TEST(ParserTest, ParsesFunctionCallNoArgs)
{
    auto expr = ParseExpr("foo()");
    auto call = std::dynamic_pointer_cast<CallExpr>(expr);
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "foo");
    EXPECT_EQ(call->arguments.size(), 0u);
}

TEST(ParserTest, ParsesFunctionCallWithArgs)
{
    auto expr = ParseExpr("add(1, 2)");
    auto call = std::dynamic_pointer_cast<CallExpr>(expr);
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "add");
    ASSERT_EQ(call->arguments.size(), 2u);
}

TEST(ParserTest, ParsesGenericFunctionCall)
{
    auto expr = ParseExpr("identity<i32>(42)");
    auto call = std::dynamic_pointer_cast<CallExpr>(expr);
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "identity");
    ASSERT_EQ(call->typeArguments.size(), 1u);
    EXPECT_EQ(call->typeArguments[0].name, "i32");
    ASSERT_EQ(call->arguments.size(), 1u);
}

TEST(ParserTest, ParsesMethodCall)
{
    auto expr = ParseExpr("obj.method(1)");
    auto method = std::dynamic_pointer_cast<MethodCallExpr>(expr);
    ASSERT_NE(method, nullptr);
    EXPECT_EQ(method->methodName, "method");
    ASSERT_EQ(method->arguments.size(), 1u);

    auto obj = std::dynamic_pointer_cast<VarExpr>(method->object);
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->name, "obj");
}

TEST(ParserTest, ParsesChainedMethodCalls)
{
    auto expr = ParseExpr("obj.a().b()");
    auto b = std::dynamic_pointer_cast<MethodCallExpr>(expr);
    ASSERT_NE(b, nullptr);
    EXPECT_EQ(b->methodName, "b");

    auto a = std::dynamic_pointer_cast<MethodCallExpr>(b->object);
    ASSERT_NE(a, nullptr);
    EXPECT_EQ(a->methodName, "a");
}

// ============================================================================
// Member access and indexing
// ============================================================================

TEST(ParserTest, ParsesMemberAccess)
{
    auto expr = ParseExpr("obj.field");
    auto member = std::dynamic_pointer_cast<MemberAccessExpr>(expr);
    ASSERT_NE(member, nullptr);
    EXPECT_EQ(member->memberName, "field");

    auto obj = std::dynamic_pointer_cast<VarExpr>(member->object);
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->name, "obj");
}

TEST(ParserTest, ParsesChainedMemberAccess)
{
    auto expr = ParseExpr("a.b.c");
    auto c = std::dynamic_pointer_cast<MemberAccessExpr>(expr);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->memberName, "c");

    auto b = std::dynamic_pointer_cast<MemberAccessExpr>(c->object);
    ASSERT_NE(b, nullptr);
    EXPECT_EQ(b->memberName, "b");
}

TEST(ParserTest, ParsesIndexExpr)
{
    auto expr = ParseExpr("arr[0]");
    auto index = std::dynamic_pointer_cast<IndexExpr>(expr);
    ASSERT_NE(index, nullptr);

    auto obj = std::dynamic_pointer_cast<VarExpr>(index->object);
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->name, "arr");
}

TEST(ParserTest, ParsesNestedIndex)
{
    auto expr = ParseExpr("matrix[0][1]");
    auto outer = std::dynamic_pointer_cast<IndexExpr>(expr);
    ASSERT_NE(outer, nullptr);

    auto inner = std::dynamic_pointer_cast<IndexExpr>(outer->object);
    ASSERT_NE(inner, nullptr);
}

// ============================================================================
// Cast, alloc, array literal
// ============================================================================

TEST(ParserTest, ParsesCastExpression)
{
    auto expr = ParseExpr("(i32) x");
    auto cast = std::dynamic_pointer_cast<CastExpr>(expr);
    ASSERT_NE(cast, nullptr);
    EXPECT_EQ(cast->targetType.name, "i32");
    EXPECT_FALSE(cast->targetType.isPointer);

    auto inner = std::dynamic_pointer_cast<VarExpr>(cast->expr);
    ASSERT_NE(inner, nullptr);
    EXPECT_EQ(inner->name, "x");
}

TEST(ParserTest, ParsesCastWithAllTypeKeywords)
{
    auto check = [](const std::string &src, const std::string &expectedType) {
        auto expr = ParseExpr(src);
        auto cast = std::dynamic_pointer_cast<CastExpr>(expr);
        ASSERT_NE(cast, nullptr) << "Failed for: " << src;
        EXPECT_EQ(cast->targetType.name, expectedType) << "Failed for: " << src;
    };

    check("(i8) x", "i8");
    check("(i16) x", "i16");
    check("(i32) x", "i32");
    check("(i64) x", "i64");
    check("(u8) x", "u8");
    check("(u16) x", "u16");
    check("(u32) x", "u32");
    check("(u64) x", "u64");
    check("(f32) x", "f32");
    check("(f64) x", "f64");
    check("(char) x", "char");
    check("(bool) x", "bool");
}

TEST(ParserTest, ParsesPointerCast)
{
    auto expr = ParseExpr("(Node*) ptr");
    auto cast = std::dynamic_pointer_cast<CastExpr>(expr);
    ASSERT_NE(cast, nullptr);
    EXPECT_EQ(cast->targetType.name, "Node");
    EXPECT_TRUE(cast->targetType.isPointer);
}

TEST(ParserTest, ParsesCastWithUnaryOperand)
{
    auto expr = ParseExpr("(i32) -x");
    auto cast = std::dynamic_pointer_cast<CastExpr>(expr);
    ASSERT_NE(cast, nullptr);
    EXPECT_EQ(cast->targetType.name, "i32");

    auto unary = std::dynamic_pointer_cast<UnaryExpr>(cast->expr);
    ASSERT_NE(unary, nullptr);
    EXPECT_EQ(unary->op, "-");
}

TEST(ParserTest, ParsesStructCast)
{
    // (struct Type) expr — explicit struct cast
    auto expr = ParseExpr("(struct MyType) x");
    auto cast = std::dynamic_pointer_cast<CastExpr>(expr);
    ASSERT_NE(cast, nullptr);
    EXPECT_EQ(cast->targetType.name, "MyType");
    EXPECT_FALSE(cast->targetType.isPointer);
}

TEST(ParserTest, ParsesAllocExpression)
{
    auto expr = ParseExpr("alloc<Node>()");
    auto alloc = std::dynamic_pointer_cast<AllocExpr>(expr);
    ASSERT_NE(alloc, nullptr);
    EXPECT_EQ(alloc->allocType.name, "Node");
    EXPECT_TRUE(alloc->allocType.isPointer); // alloc always returns pointer
}

TEST(ParserTest, ParsesArrayLiteral)
{
    auto expr = ParseExpr("[1, 2, 3]");
    auto arr = std::dynamic_pointer_cast<ArrayLiteralExpr>(expr);
    ASSERT_NE(arr, nullptr);
    ASSERT_EQ(arr->elements.size(), 3u);
}

TEST(ParserTest, ParsesEmptyArrayLiteral)
{
    auto expr = ParseExpr("[]");
    auto arr = std::dynamic_pointer_cast<ArrayLiteralExpr>(expr);
    ASSERT_NE(arr, nullptr);
    EXPECT_EQ(arr->elements.size(), 0u);
}

// ============================================================================
// Result types: Ok, Err, match
// ============================================================================

TEST(ParserTest, ParsesOkExpression)
{
    auto expr = ParseExpr("Ok(42)");
    auto ok = std::dynamic_pointer_cast<OkExpr>(expr);
    ASSERT_NE(ok, nullptr);
    ASSERT_NE(ok->value, nullptr);

    auto val = std::dynamic_pointer_cast<LiteralExpr>(ok->value);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(val->value, "42");
}

TEST(ParserTest, ParsesErrExpression)
{
    auto expr = ParseExpr("Err(1)");
    auto err = std::dynamic_pointer_cast<ErrExpr>(expr);
    ASSERT_NE(err, nullptr);
    ASSERT_NE(err->error, nullptr);
}

TEST(ParserTest, ParsesMatchExpression)
{
    std::string src = "match result { Ok(v) => v, Err(e) => 0 }";
    auto expr = ParseExpr(src);
    auto matchExpr = std::dynamic_pointer_cast<MatchExpr>(expr);
    ASSERT_NE(matchExpr, nullptr);

    EXPECT_EQ(matchExpr->okArm.pattern, "Ok");
    EXPECT_EQ(matchExpr->okArm.bindingName, "v");
    ASSERT_NE(matchExpr->okArm.body, nullptr);

    EXPECT_EQ(matchExpr->errArm.pattern, "Err");
    EXPECT_EQ(matchExpr->errArm.bindingName, "e");
    ASSERT_NE(matchExpr->errArm.body, nullptr);
}

TEST(ParserTest, ParsesMatchWithBlockArms)
{
    std::string src = "match r { Ok(v) => { return v; }, Err(e) => { return 0; } }";
    auto expr = ParseExpr(src);
    auto matchExpr = std::dynamic_pointer_cast<MatchExpr>(expr);
    ASSERT_NE(matchExpr, nullptr);

    auto okBody = std::dynamic_pointer_cast<BlockStatement>(matchExpr->okArm.body);
    ASSERT_NE(okBody, nullptr);
}

// ============================================================================
// Namespace / std::Vector
// ============================================================================

TEST(ParserTest, ParsesNamespacedConstructor)
{
    std::string src = "std::Vector<i32>()";
    auto expr = ParseExpr(src);
    auto call = std::dynamic_pointer_cast<CallExpr>(expr);
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "std::Vector");
    ASSERT_EQ(call->typeArguments.size(), 1u);
    EXPECT_EQ(call->typeArguments[0].name, "i32");
}

// ============================================================================
// Multiple top-level declarations
// ============================================================================

TEST(ParserTest, ParsesMultipleDeclarations)
{
    std::string source = "struct Point { x: i32; } fn main() -> void {}";
    auto nodes = Parse(source);

    ASSERT_EQ(nodes.size(), 2u);
    ASSERT_NE(std::dynamic_pointer_cast<StructDecl>(nodes[0]), nullptr);
    ASSERT_NE(std::dynamic_pointer_cast<FunctionDecl>(nodes[1]), nullptr);
}

TEST(ParserTest, ParsesInterfaceStructFunction)
{
    std::string source = "interface Shape { fn area() -> f64; } "
                         "struct Circle : Shape { r: f64; } "
                         "fn main() -> void {}";
    auto nodes = Parse(source);

    ASSERT_EQ(nodes.size(), 3u);
    ASSERT_NE(std::dynamic_pointer_cast<InterfaceDecl>(nodes[0]), nullptr);
    ASSERT_NE(std::dynamic_pointer_cast<StructDecl>(nodes[1]), nullptr);
    ASSERT_NE(std::dynamic_pointer_cast<FunctionDecl>(nodes[2]), nullptr);
}
