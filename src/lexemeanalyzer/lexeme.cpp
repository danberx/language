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
    if (!IsServiceWord()) return false;
    return (content == "int" || content == "double" || content == "string" || content == "bool" || content == "char" || content == "array");
}

bool Lexeme::IsFunctionType() {
    return IsType() || (IsServiceWord() && content == "void");
}

bool Lexeme::AssignmentOp() {
    if (!IsOperation()) return false;
    return (content == "=" || content == "+=" || content == "-=" || content == "*=" || content == "/=" || content == "%=");
}

bool Lexeme::EqualOp() {
    if (!IsOperation()) return false;
    return (content == "==" || content == "!=");
}

bool Lexeme::CompareOp() {
    if (!IsOperation()) return false;
    return (content == ">" || content == "<" || content == ">=" || content == "<=");
}

bool Lexeme::BitwiseShiftOp() {
    if (!IsOperation()) return false;
    return (content == "<<" || content == ">>");
}

bool Lexeme::AdditionOp() {
    if (!IsOperation()) return false;
    return (content == "+" || content == "-");
}

bool Lexeme::MultyOp() {
    if (!IsOperation()) return false;
    return (content == "*" || content == "/" || content == "%");
}

bool Lexeme::UnaryOp() {
    if (!IsOperation()) return false;
    return (content == "!" || content == "~" || content == "-" || content == "+" || content == "-" || content == "++" || content == "--");
}

bool Lexeme::PostfixOp() {
    if (!IsOperation()) return false;
    return (content == "++" || content == "--");
}

bool Lexeme::IsServiceWord() {
    return type == LexemeType::ServiceWord;
}

bool Lexeme::IsIdentifier() {
    return type == LexemeType::Identifier;
}

bool Lexeme::IsOperation() {
    return type == LexemeType::Operation;
}

bool Lexeme::IsComma() {
    return type == LexemeType::Comma;
}

bool Lexeme::IsPunctuation() {
    return type == LexemeType::Punctuation;
}

bool Lexeme::IsBracket() {
    return type == LexemeType::Bracket;
}

bool Lexeme::IsLiteral() {
    return type == LexemeType::Literal || type == LexemeType::StringLiteral || type == LexemeType::DoubleLiteral;
}

bool Lexeme::IsEnd() {
    return type == LexemeType::Other && line == -1 && content == "END";
}

bool Lexeme::IsBitwise() {
    return content == "|" || content == "&" || content == "^" || content == "<<" || content == ">>";
}

bool Lexeme::IsLogical() {
    return content == "||" || content == "&&";
}

bool Lexeme::IsEqualCompare() {
    return CompareOp() || EqualOp();
}

bool Lexeme::IsAddMulty() {
    return AdditionOp() || MultyOp();
}