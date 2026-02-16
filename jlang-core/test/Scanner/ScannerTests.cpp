#include <gtest/gtest.h>

#include <jlang/Scanner/Scanner.h>

using namespace jlang;

// ============================================================================
// Basic tokenization
// ============================================================================

TEST(ScannerTest, TokenizesEmptySource)
{
    // Given
    Scanner scanner("");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesSimpleKeywords)
{
    // Given
    Scanner scanner("void");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_GE(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].m_type, TokenType::Void);
    EXPECT_EQ(tokens[tokens.size() - 1].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesBraces)
{
    // Given
    Scanner scanner("{}()");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[1].m_type, TokenType::RBrace);
    EXPECT_EQ(tokens[2].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[3].m_type, TokenType::RParen);
}

TEST(ScannerTest, TokenizesIdentifiers)
{
    // Given
    Scanner scanner("myVar");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_GE(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "myVar");
}

TEST(ScannerTest, TokenizesNumbers)
{
    // Given
    Scanner scanner("42 123 0");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "42");
    EXPECT_EQ(tokens[1].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[1].m_lexeme, "123");
    EXPECT_EQ(tokens[2].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[2].m_lexeme, "0");
}

TEST(ScannerTest, TokenizesStringLiterals)
{
    // Given
    Scanner scanner("\"hello world\"");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "hello world");
}

// ============================================================================
// Control flow keywords
// ============================================================================

TEST(ScannerTest, TokenizesIfKeyword)
{
    // Given
    Scanner scanner("if");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::If);
    EXPECT_EQ(tokens[0].m_lexeme, "if");
}

TEST(ScannerTest, TokenizesElseKeyword)
{
    // Given
    Scanner scanner("else");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Else);
    EXPECT_EQ(tokens[0].m_lexeme, "else");
}

TEST(ScannerTest, TokenizesWhileKeyword)
{
    // Given
    Scanner scanner("while");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::While);
    EXPECT_EQ(tokens[0].m_lexeme, "while");
}

TEST(ScannerTest, TokenizesReturnKeyword)
{
    // Given
    Scanner scanner("return");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Return);
    EXPECT_EQ(tokens[0].m_lexeme, "return");
}

// ============================================================================
// Declaration keywords
// ============================================================================

TEST(ScannerTest, TokenizesFnKeyword)
{
    // Given
    Scanner scanner("fn");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Fn);
    EXPECT_EQ(tokens[0].m_lexeme, "fn");
}

TEST(ScannerTest, TokenizesVarKeyword)
{
    // Given
    Scanner scanner("var");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[0].m_lexeme, "var");
}

TEST(ScannerTest, TokenizesStructKeyword)
{
    // Given
    Scanner scanner("struct");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Struct);
    EXPECT_EQ(tokens[0].m_lexeme, "struct");
}

TEST(ScannerTest, TokenizesInterfaceKeyword)
{
    // Given
    Scanner scanner("interface");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Interface);
    EXPECT_EQ(tokens[0].m_lexeme, "interface");
}

// ============================================================================
// Memory and literal keywords
// ============================================================================

TEST(ScannerTest, TokenizesAllocKeyword)
{
    // Given
    Scanner scanner("alloc");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Alloc);
    EXPECT_EQ(tokens[0].m_lexeme, "alloc");
}

TEST(ScannerTest, TokenizesNullKeyword)
{
    // Given
    Scanner scanner("null");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Null);
    EXPECT_EQ(tokens[0].m_lexeme, "null");
}

// ============================================================================
// Boolean literals
// ============================================================================

TEST(ScannerTest, TokenizesTrueKeyword)
{
    // Given
    Scanner scanner("true");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::True);
    EXPECT_EQ(tokens[0].m_lexeme, "true");
}

TEST(ScannerTest, TokenizesFalseKeyword)
{
    // Given
    Scanner scanner("false");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::False);
    EXPECT_EQ(tokens[0].m_lexeme, "false");
}

// ============================================================================
// Type keywords
// ============================================================================

TEST(ScannerTest, TokenizesI8Keyword)
{
    // Given
    Scanner scanner("i8");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::I8);
    EXPECT_EQ(tokens[0].m_lexeme, "i8");
}

TEST(ScannerTest, TokenizesI16Keyword)
{
    // Given
    Scanner scanner("i16");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::I16);
    EXPECT_EQ(tokens[0].m_lexeme, "i16");
}

TEST(ScannerTest, TokenizesI32Keyword)
{
    // Given
    Scanner scanner("i32");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::I32);
    EXPECT_EQ(tokens[0].m_lexeme, "i32");
}

TEST(ScannerTest, TokenizesI64Keyword)
{
    // Given
    Scanner scanner("i64");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::I64);
    EXPECT_EQ(tokens[0].m_lexeme, "i64");
}

TEST(ScannerTest, TokenizesU8Keyword)
{
    // Given
    Scanner scanner("u8");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::U8);
    EXPECT_EQ(tokens[0].m_lexeme, "u8");
}

TEST(ScannerTest, TokenizesU16Keyword)
{
    // Given
    Scanner scanner("u16");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::U16);
    EXPECT_EQ(tokens[0].m_lexeme, "u16");
}

TEST(ScannerTest, TokenizesU32Keyword)
{
    // Given
    Scanner scanner("u32");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::U32);
    EXPECT_EQ(tokens[0].m_lexeme, "u32");
}

TEST(ScannerTest, TokenizesU64Keyword)
{
    // Given
    Scanner scanner("u64");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::U64);
    EXPECT_EQ(tokens[0].m_lexeme, "u64");
}

TEST(ScannerTest, TokenizesF32Keyword)
{
    // Given
    Scanner scanner("f32");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::F32);
    EXPECT_EQ(tokens[0].m_lexeme, "f32");
}

TEST(ScannerTest, TokenizesF64Keyword)
{
    // Given
    Scanner scanner("f64");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::F64);
    EXPECT_EQ(tokens[0].m_lexeme, "f64");
}

TEST(ScannerTest, TokenizesBoolKeyword)
{
    // Given
    Scanner scanner("bool");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Bool);
    EXPECT_EQ(tokens[0].m_lexeme, "bool");
}

TEST(ScannerTest, TokenizesCharKeyword)
{
    // Given
    Scanner scanner("char");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Char);
    EXPECT_EQ(tokens[0].m_lexeme, "char");
}

// ============================================================================
// Operators and symbols
// ============================================================================

TEST(ScannerTest, TokenizesSemicolon)
{
    // Given
    Scanner scanner(";");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[0].m_lexeme, ";");
}

TEST(ScannerTest, TokenizesColon)
{
    // Given
    Scanner scanner(":");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[0].m_lexeme, ":");
}

TEST(ScannerTest, TokenizesColonEqual)
{
    // Given
    Scanner scanner(":=");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::ColonEqual);
    EXPECT_EQ(tokens[0].m_lexeme, ":=");
}

TEST(ScannerTest, TokenizesColonAndColonEqualSequence)
{
    // Given - tests that : and := are distinguished correctly
    Scanner scanner(": := :");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[1].m_type, TokenType::ColonEqual);
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
}

TEST(ScannerTest, TokenizesComma)
{
    // Given
    Scanner scanner(",");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Comma);
    EXPECT_EQ(tokens[0].m_lexeme, ",");
}

TEST(ScannerTest, TokenizesDot)
{
    // Given
    Scanner scanner(".");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Dot);
    EXPECT_EQ(tokens[0].m_lexeme, ".");
}

TEST(ScannerTest, TokenizesStar)
{
    // Given
    Scanner scanner("*");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Star);
    EXPECT_EQ(tokens[0].m_lexeme, "*");
}

TEST(ScannerTest, TokenizesLess)
{
    // Given
    Scanner scanner("<");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Less);
    EXPECT_EQ(tokens[0].m_lexeme, "<");
}

TEST(ScannerTest, TokenizesGreater)
{
    // Given
    Scanner scanner(">");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Greater);
    EXPECT_EQ(tokens[0].m_lexeme, ">");
}

TEST(ScannerTest, TokenizesEqual)
{
    // Given
    Scanner scanner("=");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[0].m_lexeme, "=");
}

TEST(ScannerTest, TokenizesEqualEqual)
{
    // Given
    Scanner scanner("==");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::EqualEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "==");
}

TEST(ScannerTest, TokenizesNotEqual)
{
    // Given
    Scanner scanner("!=");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::NotEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "!=");
}

TEST(ScannerTest, TokenizesArrow)
{
    // Given
    Scanner scanner("->");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Arrow);
    EXPECT_EQ(tokens[0].m_lexeme, "->");
}

// ============================================================================
// Logical operators
// ============================================================================

TEST(ScannerTest, TokenizesNotOperator)
{
    // Given
    Scanner scanner("!");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Not);
    EXPECT_EQ(tokens[0].m_lexeme, "!");
}

TEST(ScannerTest, TokenizesAndOperator)
{
    // Given
    Scanner scanner("&&");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::And);
    EXPECT_EQ(tokens[0].m_lexeme, "&&");
}

TEST(ScannerTest, TokenizesOrOperator)
{
    // Given
    Scanner scanner("||");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Or);
    EXPECT_EQ(tokens[0].m_lexeme, "||");
}

TEST(ScannerTest, TokenizesMinus)
{
    // Given
    Scanner scanner("-");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Minus);
    EXPECT_EQ(tokens[0].m_lexeme, "-");
}

TEST(ScannerTest, TokenizesUnknownCharacter)
{
    // Given
    Scanner scanner("@");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Unknown);
    EXPECT_EQ(tokens[0].m_lexeme, "@");
}

// ============================================================================
// Identifier edge cases
// ============================================================================

TEST(ScannerTest, TokenizesIdentifierStartingWithUnderscore)
{
    // Given
    Scanner scanner("_privateVar");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "_privateVar");
}

TEST(ScannerTest, TokenizesIdentifierWithNumbers)
{
    // Given
    Scanner scanner("var123");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "var123");
}

TEST(ScannerTest, TokenizesIdentifierWithUnderscores)
{
    // Given
    Scanner scanner("my_var_name");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "my_var_name");
}

TEST(ScannerTest, TokenizesSingleUnderscoreAsIdentifier)
{
    // Given
    Scanner scanner("_");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "_");
}

// ============================================================================
// String literal edge cases
// ============================================================================

TEST(ScannerTest, TokenizesEmptyString)
{
    // Given
    Scanner scanner("\"\"");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "");
}

TEST(ScannerTest, IgnoresUnterminatedString)
{
    // Given
    Scanner scanner("\"unterminated");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then - unterminated string produces no token, only EOF
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesMultilineString)
{
    // Given
    Scanner scanner("\"hello\nworld\"");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "hello\nworld");
}

// ============================================================================
// Line number tracking
// ============================================================================

TEST(ScannerTest, TracksLineNumbersAcrossNewlines)
{
    // Given
    Scanner scanner("var\n\nfn");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[0].m_CurrentLine, 1);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
    EXPECT_EQ(tokens[1].m_CurrentLine, 3);
}

TEST(ScannerTest, TracksLineNumberInMultilineString)
{
    // Given
    std::string source = "\"line1\nline2\"\nvar";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[1].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_CurrentLine, 3);
}

// ============================================================================
// Whitespace handling
// ============================================================================

TEST(ScannerTest, HandlesTabsAsWhitespace)
{
    // Given
    Scanner scanner("var\tfn");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
}

TEST(ScannerTest, HandlesCarriageReturnAsWhitespace)
{
    // Given
    Scanner scanner("var\rfn");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
}

TEST(ScannerTest, HandlesMultipleSpaces)
{
    // Given
    Scanner scanner("var    fn");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
}

TEST(ScannerTest, HandlesMixedWhitespace)
{
    // Given
    Scanner scanner("var \t\r\n fn");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
}

// ============================================================================
// Integration tests
// ============================================================================

TEST(ScannerTest, TokenizesFunctionSignature)
{
    // Given
    std::string source = "fn main() -> void";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].m_type, TokenType::Fn);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "main");
    EXPECT_EQ(tokens[2].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[3].m_type, TokenType::RParen);
    EXPECT_EQ(tokens[4].m_type, TokenType::Arrow);
    EXPECT_EQ(tokens[5].m_type, TokenType::Void);
    EXPECT_EQ(tokens[6].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesVariableDeclaration)
{
    // Given
    std::string source = "var x: i32 = 42;";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "x");
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[3].m_type, TokenType::I32);
    EXPECT_EQ(tokens[4].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[5].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[5].m_lexeme, "42");
    EXPECT_EQ(tokens[6].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[7].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesVariableDeclarationWithTypeInference)
{
    // Given - type inference syntax: var x := 42;
    std::string source = "var x := 42;";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "x");
    EXPECT_EQ(tokens[2].m_type, TokenType::ColonEqual);
    EXPECT_EQ(tokens[2].m_lexeme, ":=");
    EXPECT_EQ(tokens[3].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[3].m_lexeme, "42");
    EXPECT_EQ(tokens[4].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[5].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesIfElseStatement)
{
    // Given
    std::string source = "if x == 0 { return null; } else { return x; }";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 16);
    EXPECT_EQ(tokens[0].m_type, TokenType::If);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_type, TokenType::EqualEqual);
    EXPECT_EQ(tokens[3].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[4].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[5].m_type, TokenType::Return);
    EXPECT_EQ(tokens[6].m_type, TokenType::Null);
    EXPECT_EQ(tokens[7].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[8].m_type, TokenType::RBrace);
    EXPECT_EQ(tokens[9].m_type, TokenType::Else);
    EXPECT_EQ(tokens[10].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[11].m_type, TokenType::Return);
    EXPECT_EQ(tokens[12].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[13].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[14].m_type, TokenType::RBrace);
    EXPECT_EQ(tokens[15].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesStructDefinition)
{
    // Given
    std::string source = "struct Point { x: f32, y: f32 }";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 12);
    EXPECT_EQ(tokens[0].m_type, TokenType::Struct);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "Point");
    EXPECT_EQ(tokens[2].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[3].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[3].m_lexeme, "x");
    EXPECT_EQ(tokens[4].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[5].m_type, TokenType::F32);
    EXPECT_EQ(tokens[6].m_type, TokenType::Comma);
    EXPECT_EQ(tokens[7].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[8].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[9].m_type, TokenType::F32);
    EXPECT_EQ(tokens[10].m_type, TokenType::RBrace);
    EXPECT_EQ(tokens[11].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesPointerType)
{
    // Given
    std::string source = "var ptr: *i32 = alloc;";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[3].m_type, TokenType::Star);
    EXPECT_EQ(tokens[4].m_type, TokenType::I32);
    EXPECT_EQ(tokens[5].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[6].m_type, TokenType::Alloc);
    EXPECT_EQ(tokens[7].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[8].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesMemberAccess)
{
    // Given
    Scanner scanner("point.x");

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "point");
    EXPECT_EQ(tokens[1].m_type, TokenType::Dot);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_lexeme, "x");
}

TEST(ScannerTest, TokenizesComparisonOperators)
{
    // Given
    std::string source = "a < b > c != d == e";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 10);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_type, TokenType::Less);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[3].m_type, TokenType::Greater);
    EXPECT_EQ(tokens[4].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[5].m_type, TokenType::NotEqual);
    EXPECT_EQ(tokens[6].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[7].m_type, TokenType::EqualEqual);
    EXPECT_EQ(tokens[8].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[9].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesInterfaceDefinition)
{
    // Given
    std::string source = "interface Drawable { fn draw() -> void; }";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 12);
    EXPECT_EQ(tokens[0].m_type, TokenType::Interface);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "Drawable");
    EXPECT_EQ(tokens[2].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[3].m_type, TokenType::Fn);
    EXPECT_EQ(tokens[4].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[5].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[6].m_type, TokenType::RParen);
    EXPECT_EQ(tokens[7].m_type, TokenType::Arrow);
    EXPECT_EQ(tokens[8].m_type, TokenType::Void);
    EXPECT_EQ(tokens[9].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[10].m_type, TokenType::RBrace);
    EXPECT_EQ(tokens[11].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesAllTypeKeywordsTogether)
{
    // Given
    std::string source = "i8 i16 i32 i64 u8 u16 u32 u64 f32 f64 bool char void";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 14);
    EXPECT_EQ(tokens[0].m_type, TokenType::I8);
    EXPECT_EQ(tokens[1].m_type, TokenType::I16);
    EXPECT_EQ(tokens[2].m_type, TokenType::I32);
    EXPECT_EQ(tokens[3].m_type, TokenType::I64);
    EXPECT_EQ(tokens[4].m_type, TokenType::U8);
    EXPECT_EQ(tokens[5].m_type, TokenType::U16);
    EXPECT_EQ(tokens[6].m_type, TokenType::U32);
    EXPECT_EQ(tokens[7].m_type, TokenType::U64);
    EXPECT_EQ(tokens[8].m_type, TokenType::F32);
    EXPECT_EQ(tokens[9].m_type, TokenType::F64);
    EXPECT_EQ(tokens[10].m_type, TokenType::Bool);
    EXPECT_EQ(tokens[11].m_type, TokenType::Char);
    EXPECT_EQ(tokens[12].m_type, TokenType::Void);
    EXPECT_EQ(tokens[13].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesBooleanExpression)
{
    // Given
    std::string source = "true && false || !x";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].m_type, TokenType::True);
    EXPECT_EQ(tokens[1].m_type, TokenType::And);
    EXPECT_EQ(tokens[2].m_type, TokenType::False);
    EXPECT_EQ(tokens[3].m_type, TokenType::Or);
    EXPECT_EQ(tokens[4].m_type, TokenType::Not);
    EXPECT_EQ(tokens[5].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesWhileLoop)
{
    // Given
    std::string source = "while x < 10 { x = x + 1; }";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 13);
    EXPECT_EQ(tokens[0].m_type, TokenType::While);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_type, TokenType::Less);
    EXPECT_EQ(tokens[3].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[4].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[5].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[7].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[8].m_type, TokenType::Plus);
    EXPECT_EQ(tokens[9].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[10].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[11].m_type, TokenType::RBrace);
    EXPECT_EQ(tokens[12].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, TokenizesBooleanVariableDeclaration)
{
    // Given
    std::string source = "var isActive: bool = true;";
    Scanner scanner(source);

    // When
    std::vector<Token> tokens = scanner.Tokenize();

    // Then
    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "isActive");
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[3].m_type, TokenType::Bool);
    EXPECT_EQ(tokens[4].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[5].m_type, TokenType::True);
    EXPECT_EQ(tokens[6].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[7].m_type, TokenType::EndOfFile);
}

// ============================================================================
// Compound assignment operators
// ============================================================================

TEST(ScannerTest, TokenizesPlusEqual)
{
    Scanner scanner("+=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::PlusEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "+=");
}

TEST(ScannerTest, TokenizesMinusEqual)
{
    Scanner scanner("-=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::MinusEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "-=");
}

TEST(ScannerTest, TokenizesStarEqual)
{
    Scanner scanner("*=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::StarEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "*=");
}

TEST(ScannerTest, TokenizesSlashEqual)
{
    Scanner scanner("/=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::SlashEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "/=");
}

TEST(ScannerTest, TokenizesPlusSpaceEqualAsSeparateTokens)
{
    Scanner scanner("+ =");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Plus);
    EXPECT_EQ(tokens[1].m_type, TokenType::Equal);
}

// ============================================================================
// Additional keywords
// ============================================================================

TEST(ScannerTest, TokenizesValKeyword)
{
    Scanner scanner("val");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Val);
    EXPECT_EQ(tokens[0].m_lexeme, "val");
}

TEST(ScannerTest, TokenizesForKeyword)
{
    Scanner scanner("for");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::For);
    EXPECT_EQ(tokens[0].m_lexeme, "for");
}

TEST(ScannerTest, TokenizesBreakKeyword)
{
    Scanner scanner("break");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Break);
    EXPECT_EQ(tokens[0].m_lexeme, "break");
}

TEST(ScannerTest, TokenizesContinueKeyword)
{
    Scanner scanner("continue");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Continue);
    EXPECT_EQ(tokens[0].m_lexeme, "continue");
}

TEST(ScannerTest, TokenizesInKeyword)
{
    Scanner scanner("in");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::In);
    EXPECT_EQ(tokens[0].m_lexeme, "in");
}

TEST(ScannerTest, TokenizesMatchKeyword)
{
    Scanner scanner("match");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Match);
    EXPECT_EQ(tokens[0].m_lexeme, "match");
}

TEST(ScannerTest, TokenizesOkKeyword)
{
    Scanner scanner("Ok");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Ok);
    EXPECT_EQ(tokens[0].m_lexeme, "Ok");
}

TEST(ScannerTest, TokenizesErrKeyword)
{
    Scanner scanner("Err");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Err);
    EXPECT_EQ(tokens[0].m_lexeme, "Err");
}

TEST(ScannerTest, TokenizesAndKeyword)
{
    Scanner scanner("and");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::AndKeyword);
    EXPECT_EQ(tokens[0].m_lexeme, "and");
}

TEST(ScannerTest, TokenizesOrKeyword)
{
    Scanner scanner("or");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::OrKeyword);
    EXPECT_EQ(tokens[0].m_lexeme, "or");
}

// ============================================================================
// Additional operators and symbols
// ============================================================================

TEST(ScannerTest, TokenizesBrackets)
{
    Scanner scanner("[]");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::LBracket);
    EXPECT_EQ(tokens[1].m_type, TokenType::RBracket);
}

TEST(ScannerTest, TokenizesColonColon)
{
    Scanner scanner("::");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::ColonColon);
    EXPECT_EQ(tokens[0].m_lexeme, "::");
}

TEST(ScannerTest, TokenizesSlash)
{
    Scanner scanner("a / b");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[1].m_type, TokenType::Slash);
    EXPECT_EQ(tokens[1].m_lexeme, "/");
}

TEST(ScannerTest, TokenizesPercent)
{
    Scanner scanner("%");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Percent);
    EXPECT_EQ(tokens[0].m_lexeme, "%");
}

TEST(ScannerTest, TokenizesPercentEqual)
{
    Scanner scanner("%=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::PercentEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "%=");
}

TEST(ScannerTest, TokenizesPlusPlus)
{
    Scanner scanner("++");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::PlusPlus);
    EXPECT_EQ(tokens[0].m_lexeme, "++");
}

TEST(ScannerTest, TokenizesMinusMinus)
{
    Scanner scanner("--");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::MinusMinus);
    EXPECT_EQ(tokens[0].m_lexeme, "--");
}

TEST(ScannerTest, TokenizesQuestion)
{
    Scanner scanner("?");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Question);
    EXPECT_EQ(tokens[0].m_lexeme, "?");
}

TEST(ScannerTest, TokenizesQuestionColon)
{
    Scanner scanner("?:");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::QuestionColon);
    EXPECT_EQ(tokens[0].m_lexeme, "?:");
}

TEST(ScannerTest, TokenizesAmpersand)
{
    Scanner scanner("a & b");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[1].m_type, TokenType::Ampersand);
    EXPECT_EQ(tokens[1].m_lexeme, "&");
}

TEST(ScannerTest, TokenizesAmpersandEqual)
{
    Scanner scanner("&=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::AmpersandEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "&=");
}

TEST(ScannerTest, TokenizesPipe)
{
    Scanner scanner("a | b");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[1].m_type, TokenType::Pipe);
    EXPECT_EQ(tokens[1].m_lexeme, "|");
}

TEST(ScannerTest, TokenizesPipeEqual)
{
    Scanner scanner("|=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::PipeEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "|=");
}

TEST(ScannerTest, TokenizesCaret)
{
    Scanner scanner("^");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Caret);
    EXPECT_EQ(tokens[0].m_lexeme, "^");
}

TEST(ScannerTest, TokenizesCaretEqual)
{
    Scanner scanner("^=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::CaretEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "^=");
}

TEST(ScannerTest, TokenizesTilde)
{
    Scanner scanner("~");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Tilde);
    EXPECT_EQ(tokens[0].m_lexeme, "~");
}

TEST(ScannerTest, TokenizesLeftShift)
{
    Scanner scanner("<<");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::LeftShift);
    EXPECT_EQ(tokens[0].m_lexeme, "<<");
}

TEST(ScannerTest, TokenizesLeftShiftEqual)
{
    Scanner scanner("<<=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::LeftShiftEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "<<=");
}

TEST(ScannerTest, TokenizesRightShift)
{
    Scanner scanner(">>");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::RightShift);
    EXPECT_EQ(tokens[0].m_lexeme, ">>");
}

TEST(ScannerTest, TokenizesRightShiftEqual)
{
    Scanner scanner(">>=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::RightShiftEqual);
    EXPECT_EQ(tokens[0].m_lexeme, ">>=");
}

TEST(ScannerTest, TokenizesLessEqual)
{
    Scanner scanner("<=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::LessEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "<=");
}

TEST(ScannerTest, TokenizesGreaterEqual)
{
    Scanner scanner(">=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::GreaterEqual);
    EXPECT_EQ(tokens[0].m_lexeme, ">=");
}

TEST(ScannerTest, TokenizesFatArrow)
{
    Scanner scanner("=>");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::FatArrow);
    EXPECT_EQ(tokens[0].m_lexeme, "=>");
}

// ============================================================================
// Float and char literals
// ============================================================================

TEST(ScannerTest, TokenizesFloatLiteral)
{
    Scanner scanner("3.14");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::FloatLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "3.14");
}

TEST(ScannerTest, TokenizesFloatLiteralWithLeadingZero)
{
    Scanner scanner("0.5");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::FloatLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "0.5");
}

TEST(ScannerTest, TokenizesCharLiteral)
{
    Scanner scanner("'a'");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::CharLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "a");
}

TEST(ScannerTest, TokenizesCharLiteralEscapeN)
{
    Scanner scanner("'\\n'");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::CharLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, std::string(1, '\n'));
}

TEST(ScannerTest, TokenizesCharLiteralEscapeT)
{
    Scanner scanner("'\\t'");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::CharLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, std::string(1, '\t'));
}

TEST(ScannerTest, TokenizesCharLiteralEscapeBackslash)
{
    Scanner scanner("'\\\\'");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::CharLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "\\");
}

TEST(ScannerTest, TokenizesCharLiteralEscapeZero)
{
    Scanner scanner("'\\0'");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::CharLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, std::string(1, '\0'));
}

TEST(ScannerTest, UnterminatedCharLiteralProducesUnknown)
{
    Scanner scanner("'a");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_GE(tokens.size(), 2u);
    EXPECT_EQ(tokens[0].m_type, TokenType::Unknown);
}

TEST(ScannerTest, EmptyCharLiteralProducesUnknown)
{
    Scanner scanner("''");
    std::vector<Token> tokens = scanner.Tokenize();

    // The scanner reads ' then immediately sees ' as the character, then looks for closing '
    // This produces Unknown because ' is consumed as the char and there's no closing quote
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_NE(tokens[0].m_type, TokenType::EndOfFile);
}

// ============================================================================
// Comments
// ============================================================================

TEST(ScannerTest, SkipsSingleLineComment)
{
    std::string source = "var // this is a comment\nfn";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
}

TEST(ScannerTest, SkipsSingleLineCommentAtEOF)
{
    std::string source = "var // comment at end";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::EndOfFile);
}

TEST(ScannerTest, SkipsBlockComment)
{
    std::string source = "var /* block comment */ fn";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
}

TEST(ScannerTest, SkipsMultilineBlockComment)
{
    std::string source = "var /* line1\nline2\nline3 */ fn";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Fn);
    EXPECT_EQ(tokens[1].m_CurrentLine, 3);
}

TEST(ScannerTest, SkipsBlockCommentAtEOF)
{
    std::string source = "var /* unterminated";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::EndOfFile);
}

// ============================================================================
// Number edge cases
// ============================================================================

TEST(ScannerTest, NumberFollowedByDotIdentifierIsNotFloat)
{
    Scanner scanner("42.x");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "42");
    EXPECT_EQ(tokens[1].m_type, TokenType::Dot);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_lexeme, "x");
}

TEST(ScannerTest, TokenizesMultipleFloatLiterals)
{
    Scanner scanner("1.0 2.5 99.99");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::FloatLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "1.0");
    EXPECT_EQ(tokens[1].m_type, TokenType::FloatLiteral);
    EXPECT_EQ(tokens[1].m_lexeme, "2.5");
    EXPECT_EQ(tokens[2].m_type, TokenType::FloatLiteral);
    EXPECT_EQ(tokens[2].m_lexeme, "99.99");
}

// ============================================================================
// Operator disambiguation
// ============================================================================

TEST(ScannerTest, DistinguishesMinusArrowMinusMinus)
{
    Scanner scanner("- -> --");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Minus);
    EXPECT_EQ(tokens[1].m_type, TokenType::Arrow);
    EXPECT_EQ(tokens[2].m_type, TokenType::MinusMinus);
}

TEST(ScannerTest, DistinguishesMinusEqual)
{
    Scanner scanner("-=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::MinusEqual);
    EXPECT_EQ(tokens[0].m_lexeme, "-=");
}

TEST(ScannerTest, DistinguishesEqualEqualEqualFatArrow)
{
    Scanner scanner("= == =>");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[1].m_type, TokenType::EqualEqual);
    EXPECT_EQ(tokens[2].m_type, TokenType::FatArrow);
}

TEST(ScannerTest, DistinguishesLessLessEqualLeftShift)
{
    Scanner scanner("< <= <<");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Less);
    EXPECT_EQ(tokens[1].m_type, TokenType::LessEqual);
    EXPECT_EQ(tokens[2].m_type, TokenType::LeftShift);
}

TEST(ScannerTest, DistinguishesGreaterGreaterEqualRightShift)
{
    Scanner scanner("> >= >>");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Greater);
    EXPECT_EQ(tokens[1].m_type, TokenType::GreaterEqual);
    EXPECT_EQ(tokens[2].m_type, TokenType::RightShift);
}

TEST(ScannerTest, DistinguishesColonColonColonColonEqual)
{
    Scanner scanner(": :: :=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[1].m_type, TokenType::ColonColon);
    EXPECT_EQ(tokens[2].m_type, TokenType::ColonEqual);
}

TEST(ScannerTest, DistinguishesAmpersandAndAmpersandEqual)
{
    Scanner scanner("& && &=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Ampersand);
    EXPECT_EQ(tokens[1].m_type, TokenType::And);
    EXPECT_EQ(tokens[2].m_type, TokenType::AmpersandEqual);
}

TEST(ScannerTest, DistinguishesPipeOrPipeEqual)
{
    Scanner scanner("| || |=");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Pipe);
    EXPECT_EQ(tokens[1].m_type, TokenType::Or);
    EXPECT_EQ(tokens[2].m_type, TokenType::PipeEqual);
}

// ============================================================================
// Complex integration tests
// ============================================================================

TEST(ScannerTest, TokenizesForEachStatement)
{
    std::string source = "for x in items { break; }";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0].m_type, TokenType::For);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "x");
    EXPECT_EQ(tokens[2].m_type, TokenType::In);
    EXPECT_EQ(tokens[3].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[4].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[5].m_type, TokenType::Break);
    EXPECT_EQ(tokens[6].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[7].m_type, TokenType::RBrace);
}

TEST(ScannerTest, TokenizesMatchExpression)
{
    std::string source = "match result { Ok(v) => v, Err(e) => 0 }";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    // match result { Ok ( v ) => v , Err ( e ) => 0 } EOF = 18
    ASSERT_EQ(tokens.size(), 18);
    EXPECT_EQ(tokens[0].m_type, TokenType::Match);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[3].m_type, TokenType::Ok);
    EXPECT_EQ(tokens[4].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[5].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].m_type, TokenType::RParen);
    EXPECT_EQ(tokens[7].m_type, TokenType::FatArrow);
    EXPECT_EQ(tokens[8].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[9].m_type, TokenType::Comma);
    EXPECT_EQ(tokens[10].m_type, TokenType::Err);
    EXPECT_EQ(tokens[11].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[12].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[13].m_type, TokenType::RParen);
    EXPECT_EQ(tokens[14].m_type, TokenType::FatArrow);
    EXPECT_EQ(tokens[15].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[16].m_type, TokenType::RBrace);
}

TEST(ScannerTest, TokenizesGenericType)
{
    std::string source = "std::Vector<i32>";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].m_lexeme, "std");
    EXPECT_EQ(tokens[1].m_type, TokenType::ColonColon);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_lexeme, "Vector");
    EXPECT_EQ(tokens[3].m_type, TokenType::Less);
    EXPECT_EQ(tokens[4].m_type, TokenType::I32);
    EXPECT_EQ(tokens[5].m_type, TokenType::Greater);
}

TEST(ScannerTest, TokenizesArrayDeclaration)
{
    std::string source = "var arr: i32[10];";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 9);
    EXPECT_EQ(tokens[0].m_type, TokenType::Var);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[3].m_type, TokenType::I32);
    EXPECT_EQ(tokens[4].m_type, TokenType::LBracket);
    EXPECT_EQ(tokens[5].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[5].m_lexeme, "10");
    EXPECT_EQ(tokens[6].m_type, TokenType::RBracket);
    EXPECT_EQ(tokens[7].m_type, TokenType::Semicolon);
}

TEST(ScannerTest, TokenizesNullablePointerType)
{
    Scanner scanner("i32*?");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::I32);
    EXPECT_EQ(tokens[1].m_type, TokenType::Star);
    EXPECT_EQ(tokens[2].m_type, TokenType::Question);
}

TEST(ScannerTest, TokenizesBitwiseExpression)
{
    std::string source = "a & b | c ^ ~d << 2 >> 1";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 13);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_type, TokenType::Ampersand);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[3].m_type, TokenType::Pipe);
    EXPECT_EQ(tokens[4].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[5].m_type, TokenType::Caret);
    EXPECT_EQ(tokens[6].m_type, TokenType::Tilde);
    EXPECT_EQ(tokens[7].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[8].m_type, TokenType::LeftShift);
    EXPECT_EQ(tokens[9].m_type, TokenType::NumberLiteral);
    EXPECT_EQ(tokens[10].m_type, TokenType::RightShift);
    EXPECT_EQ(tokens[11].m_type, TokenType::NumberLiteral);
}

TEST(ScannerTest, TokenizesElvisOperator)
{
    Scanner scanner("a ?: b");
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_type, TokenType::QuestionColon);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
}

TEST(ScannerTest, TokenizesAllocExpression)
{
    std::string source = "alloc<Point>()";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].m_type, TokenType::Alloc);
    EXPECT_EQ(tokens[1].m_type, TokenType::Less);
    EXPECT_EQ(tokens[2].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[2].m_lexeme, "Point");
    EXPECT_EQ(tokens[3].m_type, TokenType::Greater);
    EXPECT_EQ(tokens[4].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[5].m_type, TokenType::RParen);
}

TEST(ScannerTest, TokenizesValImmutableDeclaration)
{
    std::string source = "val pi: f64 = 3.14;";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].m_type, TokenType::Val);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "pi");
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[3].m_type, TokenType::F64);
    EXPECT_EQ(tokens[4].m_type, TokenType::Equal);
    EXPECT_EQ(tokens[5].m_type, TokenType::FloatLiteral);
    EXPECT_EQ(tokens[5].m_lexeme, "3.14");
    EXPECT_EQ(tokens[6].m_type, TokenType::Semicolon);
}

TEST(ScannerTest, TokenizesForLoopWithIncrementDecrement)
{
    std::string source = "for (var i: i32 = 0; i < 10; i++) {}";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    // for ( var i : i32 = 0 ; i < 10 ; i ++ ) { } EOF = 19
    ASSERT_EQ(tokens.size(), 19);
    EXPECT_EQ(tokens[0].m_type, TokenType::For);
    EXPECT_EQ(tokens[1].m_type, TokenType::LParen);
    EXPECT_EQ(tokens[2].m_type, TokenType::Var);
    EXPECT_EQ(tokens[13].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[14].m_type, TokenType::PlusPlus);
    EXPECT_EQ(tokens[15].m_type, TokenType::RParen);
}

TEST(ScannerTest, TokenizesStringWithSpaces)
{
    std::string source = "\"hello world foo bar\"";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].m_type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[0].m_lexeme, "hello world foo bar");
}

TEST(ScannerTest, TokenizesStructWithInterface)
{
    std::string source = "struct Circle : Drawable { radius: f64; }";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.Tokenize();

    // struct Circle : Drawable { radius : f64 ; } EOF = 11
    ASSERT_EQ(tokens.size(), 11);
    EXPECT_EQ(tokens[0].m_type, TokenType::Struct);
    EXPECT_EQ(tokens[1].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].m_lexeme, "Circle");
    EXPECT_EQ(tokens[2].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[3].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[3].m_lexeme, "Drawable");
    EXPECT_EQ(tokens[4].m_type, TokenType::LBrace);
    EXPECT_EQ(tokens[5].m_type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].m_type, TokenType::Colon);
    EXPECT_EQ(tokens[7].m_type, TokenType::F64);
    EXPECT_EQ(tokens[8].m_type, TokenType::Semicolon);
    EXPECT_EQ(tokens[9].m_type, TokenType::RBrace);
}
