#pragma once

#include "../lexemeanalyzer/lexicalanalyzer.hpp"

class SyntacticAnalyzer {
public:
    SyntacticAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3);
    void Programm();
    bool Type();
    void Expression();
    void Vars();
    void Var();
    void Function();
    bool Function_type();
    void Return();
    void Params();
    void Main();
    void Block();
    void Input();
    void Output();
    void Break();
    void Continue();
    void If();
    void While();
    void For();
    void Switch();
    void Cases();
    void Command();

private:
    LexicalAnalyzer lexer;
    Lexeme cur_lexeme;
};