#include "lexeme.hpp"

Lexeme::Lexeme(std::string cont, LexemeType t, int l): content(cont), type(t), line(l) {}

const std::string &Lexeme::GetContent() const {
    return content;
}

LexemeType Lexeme::GetType() const {
    return type;
}

int Lexeme::GetLine() const {
    return line;
}

bool Lexeme::IsType() {
    if (type != LexemeType::ServiceWord) return false;
    return (content == "int" || content == "double" || content == "string" || content == "bool" || content == "char" || content == "array");
}

bool Lexeme::AssignmentOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "=" || content == "+=" || content == "-=" || content == "*=" || content == "/=" || content == "%=" || content == "|=" || content == "&=" || content == "^=");
}

bool Lexeme::EqualOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "==" || content == "!=");
}

bool Lexeme::CompareOp() {
    if (type != LexemeType::Operation) return false;
    return (content == ">" || content == "<" || content == ">=" || content == "<=");
}

bool Lexeme::BitwiseShiftOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "<<" || content == ">>");
}

bool Lexeme::AdditionOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "+" || content == "-");
}

bool Lexeme::MultyOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "*" || content == "/" || content == "%");
}

bool Lexeme::UnaryOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "!" || content == "~" || content == "-" || content == "+" || content == "-" || content == "++" || content == "--");
}

bool Lexeme::PostfixOp() {
    if (type != LexemeType::Operation) return false;
    return (content == "++" || content == "--");
}