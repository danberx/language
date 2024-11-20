#pragma once

enum class LexemeType {
    ServiceWord,
    Identifier,
    Literal,
    StringLiteral,
    DoubleLiteral,
    Operation,
    Punctuation,
    Point,
    Comma,
    Bracket,
    Other
};