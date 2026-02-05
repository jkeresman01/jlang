#pragma once

#include <cstdint>
#include <sstream>
#include <string>

namespace jlang
{

enum class TokenType
{
    // Keywords
    Interface,
    Struct,
    Var,
    Val,
    Fn,
    If,
    Else,
    While,
    For,
    Return,
    Null,
    Alloc,
    True,
    False,
    Match,
    Ok,
    Err,

    // Type keywords
    Void,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    F32,
    F64,
    Bool,
    Char,

    // Symbols
    LBrace,
    RBrace,
    LParen,
    RParen,
    Semicolon,
    Colon,
    Arrow,
    Assign,
    Star,
    Plus,
    Minus,
    Slash,
    Comma,
    Dot,
    NotEqual,
    EqualEqual,
    Less,
    LessEqual, // <=
    Greater,
    GreaterEqual, // >=
    Equal,
    ColonEqual, // := for type inference
    And,
    Or,
    Not,
    AndKeyword,      // 'and' keyword (non-short-circuit)
    OrKeyword,       // 'or' keyword (non-short-circuit)
    PlusPlus,        // ++
    MinusMinus,      // --
    PlusEqual,       // +=
    MinusEqual,      // -=
    StarEqual,       // *=
    SlashEqual,      // /=
    Percent,         // %
    PercentEqual,    // %=
    Question,        // ?
    QuestionColon,   // ?:
    Ampersand,       // &
    Pipe,            // |
    Caret,           // ^
    Tilde,           // ~
    LeftShift,       // <<
    RightShift,      // >>
    AmpersandEqual,  // &=
    PipeEqual,       // |=
    CaretEqual,      // ^=
    LeftShiftEqual,  // <<=
    RightShiftEqual, // >>=
    FatArrow,        // =>

    // Literals and identifiers
    Identifier,
    StringLiteral,
    NumberLiteral,
    FloatLiteral,
    CharLiteral,

    EndOfFile,
    Unknown
};

struct Token
{
    TokenType m_type;
    std::string m_lexeme;
    uint32_t m_CurrentLine;

    Token(const TokenType type, const std::string &lexeme, uint32_t const currentLine)
        : m_type(type), m_lexeme(lexeme), m_CurrentLine(currentLine)
    {
    }

    std::string ToString() const
    {
        std::stringstream ss;

        ss << m_CurrentLine << ": ";
        ss << m_lexeme;
        ss << " (" << static_cast<int32_t>(m_type) << ")";

        return ss.str();
    }
};

} // namespace jlang
