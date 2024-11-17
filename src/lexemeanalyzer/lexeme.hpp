#pragma once

#include <string>
#include "lexemetype.hpp"

class Lexeme {
public:
    Lexeme(std::string cont, LexemeType t, int l);
    Lexeme() = default;

    const std::string& GetContent() const;
    LexemeType GetType() const;
    int GetLine() const;
    bool IsType() const;
    bool IsFunctionType() const;
    bool AssignmentOp() const;
    bool EqualOp() const;
    bool CompareOp() const;
    bool BitwiseShiftOp() const;
    bool AdditionOp() const;
    bool MultyOp() const;
    bool UnaryOp() const;
    bool PostfixOp() const;
    bool IsServiceWord() const;
    bool IsIdentifier() const;
    bool IsOperation() const;
    bool IsComma() const;
    bool IsPunctuation() const;
    bool IsBracket() const;
    bool IsLiteral() const;
    bool IsEnd() const;
private:
    std::string content;
    LexemeType type;
    int line{};
};