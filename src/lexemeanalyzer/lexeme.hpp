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
    bool AssignmentOp();
    bool EqualOp();
    bool CompareOp();
    bool BitwiseShiftOp();
    bool AdditionOp();
    bool MultyOp();
    bool UnaryOp();
    bool PostfixOp();
private:
    std::string content;
    LexemeType type;
    int line{};
};