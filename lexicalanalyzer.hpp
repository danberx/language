#pragma once

#include "lexeme.hpp"
#include "bor.hpp"

class LexicalAnalyzer {
public:
    LexicalAnalyzer(const std::string &path1, const std::string &path2);
    Lexeme GetLex() const;
private:
    void GetLexemes(const std::string &path);
    bool IsServiceWord(const std::string& s);
    bool IsIdentifier(const std::string& s);
    bool IsLiteral(const std::string& s);
    bool IsStringLiteral(const std::string& s);
    bool IsOperation(const std::string& s);
    bool IsPunctuation(const std::string& s);
    bool IsComa(const std::string& s);
    bool IsPoint(const std::string& s);
    bool IsBracket(const std::string& s);
    bool IsDouble(const std::string& s);
    bool IsInt(const std::string& s);
    Bor bor;
    std::vector<std::string> operations;
    std::vector<Lexeme> lexemes;
};
