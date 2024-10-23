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