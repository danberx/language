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

bool Lexeme::IsType() const {
    if (!IsServiceWord()) return false;
    return (content == "int" || content == "double" || content == "string" || content == "bool" || content == "char" || content == "array");
}

bool Lexeme::IsFunctionType() const {
    return IsType() || (IsServiceWord() && content == "void");
}

bool Lexeme::AssignmentOp() const {
    if (!IsOperation()) return false;
    return (content == "=" || content == "+=" || content == "-=" || content == "*=" || content == "/=" || content == "%=" || content == "|=" || content == "&=" || content == "^=");
}

bool Lexeme::EqualOp() const {
    if (!IsOperation()) return false;
    return (content == "==" || content == "!=");
}

bool Lexeme::CompareOp() const {
    if (!IsOperation()) return false;
    return (content == ">" || content == "<" || content == ">=" || content == "<=");
}

bool Lexeme::BitwiseShiftOp() const {
    if (!IsOperation()) return false;
    return (content == "<<" || content == ">>");
}

bool Lexeme::AdditionOp() const {
    if (!IsOperation()) return false;
    return (content == "+" || content == "-");
}

bool Lexeme::MultyOp() const {
    if (!IsOperation()) return false;
    return (content == "*" || content == "/" || content == "%");
}

bool Lexeme::UnaryOp() const {
    if (!IsOperation()) return false;
    return (content == "!" || content == "~" || content == "-" || content == "+" || content == "-" || content == "++" || content == "--");
}

bool Lexeme::PostfixOp() const {
    if (!IsOperation()) return false;
    return (content == "++" || content == "--");
}

bool Lexeme::IsServiceWord() const {
    return type == LexemeType::ServiceWord;
}

bool Lexeme::IsIdentifier() const {
    return type == LexemeType::Identifier;
}

bool Lexeme::IsOperation() const {
    return type == LexemeType::Operation;
}

bool Lexeme::IsComma() const {
    return type == LexemeType::Comma;
}

bool Lexeme::IsPunctuation() const {
    return type == LexemeType::Punctuation;
}

bool Lexeme::IsBracket() const {
    return type == LexemeType::Bracket;
}

bool Lexeme::IsLiteral() const {
    return type == LexemeType::Literal;
}

bool Lexeme::IsEnd() const {
    return type == LexemeType::Other && line == -1 && content == "END";
}