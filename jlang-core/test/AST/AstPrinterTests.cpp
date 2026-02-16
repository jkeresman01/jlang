#include <gtest/gtest.h>

#include <jlang/AST/AstPrinter.h>
#include <jlang/Parser/Parser.h>
#include <jlang/Scanner/Scanner.h>

using namespace jlang;

// Helper: source -> AST -> printed string
static std::string PrintAST(const std::string &source)
{
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();
    Parser parser(tokens);
    auto program = parser.Parse();
    AstPrinter printer;
    return printer.Print(program);
}

// Helper: wrap expression in a function, print, return just the expression lines
static std::string PrintExpr(const std::string &exprSource)
{
    return PrintAST("fn test() -> void { " + exprSource + "; }");
}

// ============================================================================
// Top-level declarations
// ============================================================================

TEST(AstPrinterTest, PrintsEmptyFunction)
{
    std::string result = PrintAST("fn main() -> void {}");
    std::string expected = "(FunctionDecl main [] -> void\n"
                           "  (BlockStatement\n"
                           "  )\n"
                           ")\n";
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsFunctionWithParams)
{
    std::string result = PrintAST("fn add(a: i32, b: i32) -> i32 { return a; }");
    std::string expected = "(FunctionDecl add [a:i32, b:i32] -> i32\n"
                           "  (BlockStatement\n"
                           "    (ReturnStatement\n"
                           "      (VarExpr a)\n"
                           "    )\n"
                           "  )\n"
                           ")\n";
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsFunctionWithPointerParam)
{
    std::string result = PrintAST("fn deref(p: i32*) -> i32 { return p; }");
    EXPECT_NE(result.find("[p:i32*]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericFunction)
{
    std::string result = PrintAST("fn identity<T>(x: T) -> T { return x; }");
    std::string expected = "(FunctionDecl identity<T> [x:T] -> T\n"
                           "  (BlockStatement\n"
                           "    (ReturnStatement\n"
                           "      (VarExpr x)\n"
                           "    )\n"
                           "  )\n"
                           ")\n";
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsGenericFunctionMultipleTypeParams)
{
    std::string result = PrintAST("fn pair<T, U>(a: T, b: U) -> void {}");
    EXPECT_NE(result.find("pair<T, U>"), std::string::npos);
    EXPECT_NE(result.find("[a:T, b:U]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsFunctionNullablePointerReturn)
{
    std::string result = PrintAST("fn find() -> Node*? { return null; }");
    EXPECT_NE(result.find("-> Node*?"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStructDecl)
{
    std::string result = PrintAST("struct Point { x: f64; y: f64; }");
    std::string expected = "(StructDecl Point [x:f64, y:f64])\n";
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsStructFieldVisibility)
{
    std::string result = PrintAST("struct Foo { Name: i32; value: i32; }");
    EXPECT_NE(result.find("Name:i32 pub"), std::string::npos);
    // Private fields have no suffix
    EXPECT_NE(result.find("value:i32]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStructWithInterface)
{
    std::string result = PrintAST("struct Circle : Drawable { radius: f64; }");
    EXPECT_NE(result.find("implements Drawable"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericStruct)
{
    std::string result = PrintAST("struct Pair<T, U> { first: T; second: U; }");
    std::string expected = "(StructDecl Pair<T, U> [first:T, second:U])\n";
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsStructWithPointerField)
{
    std::string result = PrintAST("struct Node { next: Node*; }");
    EXPECT_NE(result.find("next:Node*"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStructWithNullablePointerField)
{
    std::string result = PrintAST("struct Node { next: Node*?; }");
    EXPECT_NE(result.find("next:Node*?"), std::string::npos);
}

TEST(AstPrinterTest, PrintsInterfaceDecl)
{
    std::string result = PrintAST("interface Drawable { fn draw(x: i32) -> void; }");
    std::string expected = "(InterfaceDecl Drawable\n"
                           "  fn draw(x:i32) -> void\n"
                           ")\n";
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsInterfaceMultipleMethods)
{
    std::string result = PrintAST("interface Shape { fn area() -> f64; fn name() -> i32; }");
    EXPECT_NE(result.find("fn area() -> f64"), std::string::npos);
    EXPECT_NE(result.find("fn name() -> i32"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMultipleDeclarations)
{
    std::string result = PrintAST("struct Point { x: i32; } fn main() -> void {}");
    EXPECT_NE(result.find("(StructDecl Point"), std::string::npos);
    EXPECT_NE(result.find("(FunctionDecl main"), std::string::npos);
}

// ============================================================================
// Variable declarations
// ============================================================================

TEST(AstPrinterTest, PrintsVarDeclMutable)
{
    std::string result = PrintExpr("var x: i32 = 42");
    EXPECT_NE(result.find("(VariableDecl x : i32 mut"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsValDeclImmutable)
{
    std::string result = PrintExpr("val x: i32 = 10");
    EXPECT_NE(result.find("(VariableDecl x : i32 immut"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarDeclNoInitializer)
{
    std::string result = PrintExpr("var x: i32");
    EXPECT_NE(result.find("(VariableDecl x : i32 mut"), std::string::npos);
    EXPECT_NE(result.find("<null>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarDeclArrayType)
{
    std::string result = PrintExpr("var arr: i32[10]");
    EXPECT_NE(result.find("i32[10]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarDeclGenericType)
{
    std::string src = "fn test() -> void { var r: Result<i32, char*>; }";
    std::string result = PrintAST(src);
    EXPECT_NE(result.find("Result<i32, char*>"), std::string::npos);
}

// ============================================================================
// Statements
// ============================================================================

TEST(AstPrinterTest, PrintsReturnWithValue)
{
    std::string result = PrintExpr("return 42");
    EXPECT_NE(result.find("(ReturnStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsReturnVoid)
{
    std::string result = PrintAST("fn test() -> void { return; }");
    EXPECT_NE(result.find("(ReturnStatement\n"), std::string::npos);
    EXPECT_NE(result.find("<null>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsBreakStatement)
{
    std::string result = PrintExpr("break");
    EXPECT_NE(result.find("(BreakStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsContinueStatement)
{
    std::string result = PrintExpr("continue");
    EXPECT_NE(result.find("(ContinueStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIfStatement)
{
    std::string result = PrintAST("fn test() -> void { if (x) { return; } }");
    EXPECT_NE(result.find("(IfStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIfElseStatement)
{
    std::string src = "fn test() -> void { if (x) { return; } else { break; } }";
    std::string result = PrintAST(src);
    std::string ifStr = "(IfStatement\n";
    // Should contain both then and else branches
    EXPECT_NE(result.find("(ReturnStatement"), std::string::npos);
    EXPECT_NE(result.find("(BreakStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsWhileStatement)
{
    std::string result = PrintAST("fn test() -> void { while (x) { break; } }");
    EXPECT_NE(result.find("(WhileStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
    EXPECT_NE(result.find("(BreakStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsForStatement)
{
    std::string src = "fn test() -> void { for (var i: i32 = 0; i < 10; i++) {} }";
    std::string result = PrintAST(src);
    EXPECT_NE(result.find("(ForStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(VariableDecl i"), std::string::npos);
    EXPECT_NE(result.find("(BinaryExpr <"), std::string::npos);
    EXPECT_NE(result.find("(PostfixExpr ++"), std::string::npos);
}

TEST(AstPrinterTest, PrintsForStatementEmptyClauses)
{
    std::string result = PrintAST("fn test() -> void { for (;;) { break; } }");
    EXPECT_NE(result.find("(ForStatement\n"), std::string::npos);
    // All three clauses should be <null>
    size_t pos = 0;
    int nullCount = 0;
    while ((pos = result.find("<null>", pos)) != std::string::npos)
    {
        nullCount++;
        pos += 6;
    }
    EXPECT_GE(nullCount, 3); // init, condition, update all null
}

TEST(AstPrinterTest, PrintsForEachStatement)
{
    std::string result = PrintAST("fn test() -> void { for elem in items {} }");
    EXPECT_NE(result.find("(ForEachStatement elem in"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr items)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsBlockStatement)
{
    std::string result = PrintAST("fn test() -> void { { var x: i32 = 1; } }");
    // Outer block + inner block
    size_t first = result.find("(BlockStatement");
    size_t second = result.find("(BlockStatement", first + 1);
    EXPECT_NE(first, std::string::npos);
    EXPECT_NE(second, std::string::npos);
}

TEST(AstPrinterTest, PrintsExprStatement)
{
    std::string result = PrintExpr("foo()");
    EXPECT_NE(result.find("(ExprStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(CallExpr foo"), std::string::npos);
}

// ============================================================================
// Literal expressions
// ============================================================================

TEST(AstPrinterTest, PrintsIntegerLiteral)
{
    std::string result = PrintExpr("42");
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsFloatLiteral)
{
    std::string result = PrintExpr("3.14");
    EXPECT_NE(result.find("(LiteralExpr 3.14)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsBoolLiterals)
{
    std::string t = PrintExpr("true");
    std::string f = PrintExpr("false");
    EXPECT_NE(t.find("(LiteralExpr true)"), std::string::npos);
    EXPECT_NE(f.find("(LiteralExpr false)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsNullLiteral)
{
    std::string result = PrintExpr("null");
    EXPECT_NE(result.find("(LiteralExpr null)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStringLiteral)
{
    std::string result = PrintExpr("\"hello\"");
    EXPECT_NE(result.find("(LiteralExpr \"hello\")"), std::string::npos);
}

TEST(AstPrinterTest, PrintsCharLiteral)
{
    std::string result = PrintExpr("'a'");
    EXPECT_NE(result.find("(LiteralExpr 'a')"), std::string::npos);
}

// ============================================================================
// Binary expressions
// ============================================================================

TEST(AstPrinterTest, PrintsBinaryExpr)
{
    std::string result = PrintExpr("a + b");
    std::string expected_fragment = "(BinaryExpr +\n"
                                    "        (VarExpr a)\n"
                                    "        (VarExpr b)\n"
                                    "      )";
    EXPECT_NE(result.find("(BinaryExpr +"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr a)"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr b)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsNestedBinaryExpr)
{
    // a + b * c => (+ a (* b c))
    std::string result = PrintExpr("a + b * c");
    // The outer op should be +, inner should be *
    size_t plusPos = result.find("(BinaryExpr +");
    size_t mulPos = result.find("(BinaryExpr *");
    EXPECT_NE(plusPos, std::string::npos);
    EXPECT_NE(mulPos, std::string::npos);
    // * should be nested inside +
    EXPECT_LT(plusPos, mulPos);
}

TEST(AstPrinterTest, PrintsAllBinaryOperators)
{
    auto check = [](const std::string &op) {
        std::string result = PrintExpr("a " + op + " b");
        EXPECT_NE(result.find("(BinaryExpr " + op), std::string::npos) << "Failed for operator: " << op;
    };

    check("+");
    check("-");
    check("*");
    check("/");
    check("%");
    check("==");
    check("!=");
    check("<");
    check(">");
    check("<=");
    check(">=");
    check("&&");
    check("||");
    check("&");
    check("|");
    check("^");
    check("<<");
    check(">>");
}

TEST(AstPrinterTest, PrintsElvisOperator)
{
    std::string result = PrintExpr("a ?: b");
    EXPECT_NE(result.find("(BinaryExpr ?:"), std::string::npos);
}

// ============================================================================
// Unary, prefix, postfix expressions
// ============================================================================

TEST(AstPrinterTest, PrintsUnaryNegation)
{
    std::string result = PrintExpr("-x");
    EXPECT_NE(result.find("(UnaryExpr -"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsUnaryNot)
{
    std::string result = PrintExpr("!x");
    EXPECT_NE(result.find("(UnaryExpr !"), std::string::npos);
}

TEST(AstPrinterTest, PrintsUnaryBitwiseNot)
{
    std::string result = PrintExpr("~x");
    EXPECT_NE(result.find("(UnaryExpr ~"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPrefixIncrement)
{
    std::string result = PrintExpr("++x");
    EXPECT_NE(result.find("(PrefixExpr ++"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPrefixDecrement)
{
    std::string result = PrintExpr("--x");
    EXPECT_NE(result.find("(PrefixExpr --"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPostfixIncrement)
{
    std::string result = PrintExpr("x++");
    EXPECT_NE(result.find("(PostfixExpr ++"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPostfixDecrement)
{
    std::string result = PrintExpr("x--");
    EXPECT_NE(result.find("(PostfixExpr --"), std::string::npos);
}

// ============================================================================
// Assignment expressions
// ============================================================================

TEST(AstPrinterTest, PrintsAssignExpr)
{
    std::string result = PrintExpr("x = 42");
    EXPECT_NE(result.find("(AssignExpr x"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsCompoundAssignment)
{
    // x += 1 is desugared to x = x + 1
    std::string result = PrintExpr("x += 1");
    EXPECT_NE(result.find("(AssignExpr x"), std::string::npos);
    EXPECT_NE(result.find("(BinaryExpr +"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIndexAssignExpr)
{
    std::string result = PrintExpr("arr[0] = 42");
    EXPECT_NE(result.find("(IndexAssignExpr"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr arr)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 0)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMemberAssignExpr)
{
    std::string result = PrintExpr("obj.field = 42");
    EXPECT_NE(result.find("(MemberAssignExpr .field ="), std::string::npos);
    EXPECT_NE(result.find("(VarExpr obj)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

// ============================================================================
// Call expressions
// ============================================================================

TEST(AstPrinterTest, PrintsCallExprNoArgs)
{
    std::string result = PrintExpr("foo()");
    std::string expected_fragment = "(CallExpr foo\n"
                                    "      )\n";
    EXPECT_NE(result.find("(CallExpr foo"), std::string::npos);
}

TEST(AstPrinterTest, PrintsCallExprWithArgs)
{
    std::string result = PrintExpr("add(1, 2)");
    EXPECT_NE(result.find("(CallExpr add"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 2)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericCallExpr)
{
    std::string result = PrintExpr("identity<i32>(42)");
    EXPECT_NE(result.find("(CallExpr identity<i32>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericCallMultipleTypeArgs)
{
    std::string result = PrintExpr("make<i32, f64>(1, 2)");
    EXPECT_NE(result.find("(CallExpr make<i32, f64>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMethodCallExpr)
{
    std::string result = PrintExpr("obj.method(1)");
    EXPECT_NE(result.find("(MethodCallExpr .method"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr obj)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsChainedMethodCalls)
{
    std::string result = PrintExpr("obj.a().b()");
    EXPECT_NE(result.find("(MethodCallExpr .b"), std::string::npos);
    EXPECT_NE(result.find("(MethodCallExpr .a"), std::string::npos);
}

// ============================================================================
// Member access and indexing
// ============================================================================

TEST(AstPrinterTest, PrintsMemberAccessExpr)
{
    std::string result = PrintExpr("obj.field");
    EXPECT_NE(result.find("(MemberAccessExpr .field"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr obj)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsChainedMemberAccess)
{
    std::string result = PrintExpr("a.b.c");
    EXPECT_NE(result.find("(MemberAccessExpr .c"), std::string::npos);
    EXPECT_NE(result.find("(MemberAccessExpr .b"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr a)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIndexExpr)
{
    std::string result = PrintExpr("arr[0]");
    EXPECT_NE(result.find("(IndexExpr"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr arr)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 0)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsNestedIndex)
{
    std::string result = PrintExpr("matrix[0][1]");
    // Outer IndexExpr contains inner IndexExpr
    size_t first = result.find("(IndexExpr");
    size_t second = result.find("(IndexExpr", first + 1);
    EXPECT_NE(first, std::string::npos);
    EXPECT_NE(second, std::string::npos);
}

// ============================================================================
// Cast, alloc, array literal
// ============================================================================

TEST(AstPrinterTest, PrintsCastExpr)
{
    std::string result = PrintExpr("(i32) x");
    EXPECT_NE(result.find("(CastExpr -> i32"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPointerCast)
{
    std::string result = PrintExpr("(Node*) ptr");
    EXPECT_NE(result.find("(CastExpr -> Node*"), std::string::npos);
}

TEST(AstPrinterTest, PrintsAllocExpr)
{
    std::string result = PrintExpr("alloc<Node>()");
    EXPECT_NE(result.find("(AllocExpr Node*)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsArrayLiteralExpr)
{
    std::string result = PrintExpr("[1, 2, 3]");
    EXPECT_NE(result.find("(ArrayLiteralExpr [3]"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 2)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 3)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsEmptyArrayLiteral)
{
    std::string result = PrintExpr("[]");
    EXPECT_NE(result.find("(ArrayLiteralExpr [0]"), std::string::npos);
}

// ============================================================================
// Result types: Ok, Err, Match
// ============================================================================

TEST(AstPrinterTest, PrintsOkExpr)
{
    std::string result = PrintExpr("Ok(42)");
    EXPECT_NE(result.find("(OkExpr"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsErrExpr)
{
    std::string result = PrintExpr("Err(1)");
    EXPECT_NE(result.find("(ErrExpr"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMatchExpr)
{
    std::string src = "match result { Ok(v) => v, Err(e) => 0 }";
    std::string result = PrintExpr(src);
    EXPECT_NE(result.find("(MatchExpr"), std::string::npos);
    EXPECT_NE(result.find("scrutinee:"), std::string::npos);
    EXPECT_NE(result.find("Ok(v) =>"), std::string::npos);
    EXPECT_NE(result.find("Err(e) =>"), std::string::npos);
}

// ============================================================================
// Indentation
// ============================================================================

TEST(AstPrinterTest, IndentationIncreasesWithNesting)
{
    std::string result = PrintAST("fn main() -> void { if (x) { return 1; } }");
    // Top-level: no indent
    EXPECT_EQ(result.find("(FunctionDecl"), 0u);
    // Body block: 2 spaces
    EXPECT_NE(result.find("  (BlockStatement"), std::string::npos);
    // If inside block: 4 spaces
    EXPECT_NE(result.find("    (IfStatement"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarExpr)
{
    std::string result = PrintExpr("x");
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

// ============================================================================
// Namespace / std::Vector
// ============================================================================

TEST(AstPrinterTest, PrintsNamespacedConstructor)
{
    std::string result = PrintExpr("std::Vector<i32>()");
    EXPECT_NE(result.find("(CallExpr std::Vector<i32>"), std::string::npos);
}

// ============================================================================
// Full program round-trip
// ============================================================================

TEST(AstPrinterTest, PrintsCompleteProgram)
{
    std::string source = "struct Point { x: f64; y: f64; } "
                         "fn distance(p: Point*) -> f64 { return 0; }";
    std::string result = PrintAST(source);

    // Both declarations present
    EXPECT_NE(result.find("(StructDecl Point"), std::string::npos);
    EXPECT_NE(result.find("(FunctionDecl distance"), std::string::npos);

    // Struct printed before function
    size_t structPos = result.find("(StructDecl");
    size_t funcPos = result.find("(FunctionDecl");
    EXPECT_LT(structPos, funcPos);
}

TEST(AstPrinterTest, PrintsEmptyProgram)
{
    std::string result = PrintAST("");
    EXPECT_EQ(result, "");
}
