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
    // Given
    std::string source = "fn main() -> void {}";
    std::string expected = "(FunctionDecl main [] -> void\n"
                           "  (BlockStatement\n"
                           "  )\n"
                           ")\n";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsFunctionWithParams)
{
    // Given
    std::string source = "fn add(a: i32, b: i32) -> i32 { return a; }";
    std::string expected = "(FunctionDecl add [a:i32, b:i32] -> i32\n"
                           "  (BlockStatement\n"
                           "    (ReturnStatement\n"
                           "      (VarExpr a)\n"
                           "    )\n"
                           "  )\n"
                           ")\n";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsFunctionWithPointerParam)
{
    // Given
    std::string source = "fn deref(p: i32*) -> i32 { return p; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("[p:i32*]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericFunction)
{
    // Given
    std::string source = "fn identity<T>(x: T) -> T { return x; }";
    std::string expected = "(FunctionDecl identity<T> [x:T] -> T\n"
                           "  (BlockStatement\n"
                           "    (ReturnStatement\n"
                           "      (VarExpr x)\n"
                           "    )\n"
                           "  )\n"
                           ")\n";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsGenericFunctionMultipleTypeParams)
{
    // Given
    std::string source = "fn pair<T, U>(a: T, b: U) -> void {}";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("pair<T, U>"), std::string::npos);
    EXPECT_NE(result.find("[a:T, b:U]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsFunctionNullablePointerReturn)
{
    // Given
    std::string source = "fn find() -> Node*? { return null; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("-> Node*?"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStructDecl)
{
    // Given
    std::string source = "struct Point { x: f64; y: f64; }";
    std::string expected = "(StructDecl Point [x:f64, y:f64])\n";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsStructFieldVisibility)
{
    // Given
    std::string source = "struct Foo { Name: i32; value: i32; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("Name:i32 pub"), std::string::npos);
    // Private fields have no suffix
    EXPECT_NE(result.find("value:i32]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStructWithInterface)
{
    // Given
    std::string source = "struct Circle : Drawable { radius: f64; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("implements Drawable"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericStruct)
{
    // Given
    std::string source = "struct Pair<T, U> { first: T; second: U; }";
    std::string expected = "(StructDecl Pair<T, U> [first:T, second:U])\n";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsStructWithPointerField)
{
    // Given
    std::string source = "struct Node { next: Node*; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("next:Node*"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStructWithNullablePointerField)
{
    // Given
    std::string source = "struct Node { next: Node*?; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("next:Node*?"), std::string::npos);
}

TEST(AstPrinterTest, PrintsInterfaceDecl)
{
    // Given
    std::string source = "interface Drawable { fn draw(x: i32) -> void; }";
    std::string expected = "(InterfaceDecl Drawable\n"
                           "  fn draw(x:i32) -> void\n"
                           ")\n";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, expected);
}

TEST(AstPrinterTest, PrintsInterfaceMultipleMethods)
{
    // Given
    std::string source = "interface Shape { fn area() -> f64; fn name() -> i32; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("fn area() -> f64"), std::string::npos);
    EXPECT_NE(result.find("fn name() -> i32"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMultipleDeclarations)
{
    // Given
    std::string source = "struct Point { x: i32; } fn main() -> void {}";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("(StructDecl Point"), std::string::npos);
    EXPECT_NE(result.find("(FunctionDecl main"), std::string::npos);
}

// ============================================================================
// Variable declarations
// ============================================================================

TEST(AstPrinterTest, PrintsVarDeclMutable)
{
    // Given
    std::string source = "var x: i32 = 42";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(VariableDecl x : i32 mut"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsValDeclImmutable)
{
    // Given
    std::string source = "val x: i32 = 10";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(VariableDecl x : i32 immut"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarDeclNoInitializer)
{
    // Given
    std::string source = "var x: i32";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(VariableDecl x : i32 mut"), std::string::npos);
    EXPECT_NE(result.find("<null>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarDeclArrayType)
{
    // Given
    std::string source = "var arr: i32[10]";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("i32[10]"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarDeclGenericType)
{
    // Given
    std::string source = "fn test() -> void { var r: Result<i32, char*>; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("Result<i32, char*>"), std::string::npos);
}

// ============================================================================
// Statements
// ============================================================================

TEST(AstPrinterTest, PrintsReturnWithValue)
{
    // Given
    std::string source = "return 42";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(ReturnStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsReturnVoid)
{
    // Given
    std::string source = "fn test() -> void { return; }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("(ReturnStatement\n"), std::string::npos);
    EXPECT_NE(result.find("<null>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsBreakStatement)
{
    // Given
    std::string source = "break";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(BreakStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsContinueStatement)
{
    // Given
    std::string source = "continue";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(ContinueStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIfStatement)
{
    // Given
    std::string source = "fn test() -> void { if (x) { return; } }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("(IfStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIfElseStatement)
{
    // Given
    std::string source = "fn test() -> void { if (x) { return; } else { break; } }";

    // When
    std::string result = PrintAST(source);

    // Then
    // Should contain both then and else branches
    EXPECT_NE(result.find("(ReturnStatement"), std::string::npos);
    EXPECT_NE(result.find("(BreakStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsWhileStatement)
{
    // Given
    std::string source = "fn test() -> void { while (x) { break; } }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("(WhileStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
    EXPECT_NE(result.find("(BreakStatement)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsForStatement)
{
    // Given
    std::string source = "fn test() -> void { for (var i: i32 = 0; i < 10; i++) {} }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("(ForStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(VariableDecl i"), std::string::npos);
    EXPECT_NE(result.find("(BinaryExpr <"), std::string::npos);
    EXPECT_NE(result.find("(PostfixExpr ++"), std::string::npos);
}

TEST(AstPrinterTest, PrintsForStatementEmptyClauses)
{
    // Given
    std::string source = "fn test() -> void { for (;;) { break; } }";

    // When
    std::string result = PrintAST(source);

    // Then
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
    // Given
    std::string source = "fn test() -> void { for elem in items {} }";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_NE(result.find("(ForEachStatement elem in"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr items)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsBlockStatement)
{
    // Given
    std::string source = "fn test() -> void { { var x: i32 = 1; } }";

    // When
    std::string result = PrintAST(source);

    // Then
    // Outer block + inner block
    size_t first = result.find("(BlockStatement");
    size_t second = result.find("(BlockStatement", first + 1);
    EXPECT_NE(first, std::string::npos);
    EXPECT_NE(second, std::string::npos);
}

TEST(AstPrinterTest, PrintsExprStatement)
{
    // Given
    std::string source = "foo()";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(ExprStatement\n"), std::string::npos);
    EXPECT_NE(result.find("(CallExpr foo"), std::string::npos);
}

// ============================================================================
// Literal expressions
// ============================================================================

TEST(AstPrinterTest, PrintsIntegerLiteral)
{
    // Given
    std::string source = "42";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsFloatLiteral)
{
    // Given
    std::string source = "3.14";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(LiteralExpr 3.14)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsBoolLiterals)
{
    // Given
    std::string sourceTrue = "true";
    std::string sourceFalse = "false";

    // When
    std::string t = PrintExpr(sourceTrue);
    std::string f = PrintExpr(sourceFalse);

    // Then
    EXPECT_NE(t.find("(LiteralExpr true)"), std::string::npos);
    EXPECT_NE(f.find("(LiteralExpr false)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsNullLiteral)
{
    // Given
    std::string source = "null";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(LiteralExpr null)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsStringLiteral)
{
    // Given
    std::string source = "\"hello\"";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(LiteralExpr \"hello\")"), std::string::npos);
}

TEST(AstPrinterTest, PrintsCharLiteral)
{
    // Given
    std::string source = "'a'";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(LiteralExpr 'a')"), std::string::npos);
}

// ============================================================================
// Binary expressions
// ============================================================================

TEST(AstPrinterTest, PrintsBinaryExpr)
{
    // Given
    std::string source = "a + b";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(BinaryExpr +"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr a)"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr b)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsNestedBinaryExpr)
{
    // Given
    // a + b * c => (+ a (* b c))
    std::string source = "a + b * c";

    // When
    std::string result = PrintExpr(source);

    // Then
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
    // Given
    auto check = [](const std::string &op) {
        std::string result = PrintExpr("a " + op + " b");
        EXPECT_NE(result.find("(BinaryExpr " + op), std::string::npos) << "Failed for operator: " << op;
    };

    // When / Then
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
    // Given
    std::string source = "a ?: b";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(BinaryExpr ?:"), std::string::npos);
}

// ============================================================================
// Unary, prefix, postfix expressions
// ============================================================================

TEST(AstPrinterTest, PrintsUnaryNegation)
{
    // Given
    std::string source = "-x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(UnaryExpr -"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsUnaryNot)
{
    // Given
    std::string source = "!x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(UnaryExpr !"), std::string::npos);
}

TEST(AstPrinterTest, PrintsUnaryBitwiseNot)
{
    // Given
    std::string source = "~x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(UnaryExpr ~"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPrefixIncrement)
{
    // Given
    std::string source = "++x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(PrefixExpr ++"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPrefixDecrement)
{
    // Given
    std::string source = "--x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(PrefixExpr --"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPostfixIncrement)
{
    // Given
    std::string source = "x++";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(PostfixExpr ++"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPostfixDecrement)
{
    // Given
    std::string source = "x--";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(PostfixExpr --"), std::string::npos);
}

// ============================================================================
// Assignment expressions
// ============================================================================

TEST(AstPrinterTest, PrintsAssignExpr)
{
    // Given
    std::string source = "x = 42";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(AssignExpr x"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsCompoundAssignment)
{
    // Given
    // x += 1 is desugared to x = x + 1
    std::string source = "x += 1";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(AssignExpr x"), std::string::npos);
    EXPECT_NE(result.find("(BinaryExpr +"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIndexAssignExpr)
{
    // Given
    std::string source = "arr[0] = 42";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(IndexAssignExpr"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr arr)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 0)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMemberAssignExpr)
{
    // Given
    std::string source = "obj.field = 42";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(MemberAssignExpr .field ="), std::string::npos);
    EXPECT_NE(result.find("(VarExpr obj)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

// ============================================================================
// Call expressions
// ============================================================================

TEST(AstPrinterTest, PrintsCallExprNoArgs)
{
    // Given
    std::string source = "foo()";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CallExpr foo"), std::string::npos);
}

TEST(AstPrinterTest, PrintsCallExprWithArgs)
{
    // Given
    std::string source = "add(1, 2)";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CallExpr add"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 2)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericCallExpr)
{
    // Given
    std::string source = "identity<i32>(42)";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CallExpr identity<i32>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsGenericCallMultipleTypeArgs)
{
    // Given
    std::string source = "make<i32, f64>(1, 2)";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CallExpr make<i32, f64>"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMethodCallExpr)
{
    // Given
    std::string source = "obj.method(1)";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(MethodCallExpr .method"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr obj)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsChainedMethodCalls)
{
    // Given
    std::string source = "obj.a().b()";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(MethodCallExpr .b"), std::string::npos);
    EXPECT_NE(result.find("(MethodCallExpr .a"), std::string::npos);
}

// ============================================================================
// Member access and indexing
// ============================================================================

TEST(AstPrinterTest, PrintsMemberAccessExpr)
{
    // Given
    std::string source = "obj.field";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(MemberAccessExpr .field"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr obj)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsChainedMemberAccess)
{
    // Given
    std::string source = "a.b.c";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(MemberAccessExpr .c"), std::string::npos);
    EXPECT_NE(result.find("(MemberAccessExpr .b"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr a)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsIndexExpr)
{
    // Given
    std::string source = "arr[0]";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(IndexExpr"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr arr)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 0)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsNestedIndex)
{
    // Given
    std::string source = "matrix[0][1]";

    // When
    std::string result = PrintExpr(source);

    // Then
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
    // Given
    std::string source = "(i32) x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CastExpr -> i32"), std::string::npos);
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsPointerCast)
{
    // Given
    std::string source = "(Node*) ptr";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CastExpr -> Node*"), std::string::npos);
}

TEST(AstPrinterTest, PrintsAllocExpr)
{
    // Given
    std::string source = "alloc<Node>()";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(AllocExpr Node*)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsArrayLiteralExpr)
{
    // Given
    std::string source = "[1, 2, 3]";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(ArrayLiteralExpr [3]"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 2)"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 3)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsEmptyArrayLiteral)
{
    // Given
    std::string source = "[]";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(ArrayLiteralExpr [0]"), std::string::npos);
}

// ============================================================================
// Result types: Ok, Err, Match
// ============================================================================

TEST(AstPrinterTest, PrintsOkExpr)
{
    // Given
    std::string source = "Ok(42)";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(OkExpr"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 42)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsErrExpr)
{
    // Given
    std::string source = "Err(1)";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(ErrExpr"), std::string::npos);
    EXPECT_NE(result.find("(LiteralExpr 1)"), std::string::npos);
}

TEST(AstPrinterTest, PrintsMatchExpr)
{
    // Given
    std::string source = "match result { Ok(v) => v, Err(e) => 0 }";

    // When
    std::string result = PrintExpr(source);

    // Then
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
    // Given
    std::string source = "fn main() -> void { if (x) { return 1; } }";

    // When
    std::string result = PrintAST(source);

    // Then
    // Top-level: no indent
    EXPECT_EQ(result.find("(FunctionDecl"), 0u);
    // Body block: 2 spaces
    EXPECT_NE(result.find("  (BlockStatement"), std::string::npos);
    // If inside block: 4 spaces
    EXPECT_NE(result.find("    (IfStatement"), std::string::npos);
}

TEST(AstPrinterTest, PrintsVarExpr)
{
    // Given
    std::string source = "x";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(VarExpr x)"), std::string::npos);
}

// ============================================================================
// Namespace / std::Vector
// ============================================================================

TEST(AstPrinterTest, PrintsNamespacedConstructor)
{
    // Given
    std::string source = "std::Vector<i32>()";

    // When
    std::string result = PrintExpr(source);

    // Then
    EXPECT_NE(result.find("(CallExpr std::Vector<i32>"), std::string::npos);
}

// ============================================================================
// Full program round-trip
// ============================================================================

TEST(AstPrinterTest, PrintsCompleteProgram)
{
    // Given
    std::string source = "struct Point { x: f64; y: f64; } "
                         "fn distance(p: Point*) -> f64 { return 0; }";

    // When
    std::string result = PrintAST(source);

    // Then
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
    // Given
    std::string source = "";

    // When
    std::string result = PrintAST(source);

    // Then
    EXPECT_EQ(result, "");
}
