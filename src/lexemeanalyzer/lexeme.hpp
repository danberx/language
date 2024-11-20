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
    bool IsType();
    bool IsFunctionType();
    bool AssignmentOp();
    bool EqualOp();
    bool CompareOp();
    bool BitwiseShiftOp();
    bool AdditionOp();
    bool MultyOp();
    bool UnaryOp();
    bool PostfixOp();
    bool IsServiceWord();
    bool IsIdentifier();
    bool IsOperation();
    bool IsComma();
    bool IsPunctuation();
    bool IsBracket();
    bool IsLiteral();
    bool IsEnd();
    bool IsBitwise();
    bool IsLogical();
    bool IsEqualCompare();
    bool IsAddMulty();
private:
    std::string content;
    LexemeType type;
    int line{};
};