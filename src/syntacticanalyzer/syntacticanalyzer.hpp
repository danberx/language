#pragma once
#include <iostream>
#include "semanticanalyzer.hpp"
#include "../lexemeanalyzer/lexicalanalyzer.hpp"
#include "../generation/poliz.hpp"

class SyntacticAnalyzer {
public:
    SyntacticAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3);
    void Programm();
    void Expression();
    void Vars();
    Type Var();
    void Function();
    void Return();
    void Params(std::vector<Type>& args);
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
    void Assignment_exp();
    void Logical_or_exp();
    void Logical_and_exp();
    void Bitwise_or_exp();
    void Bitwise_xor_exp();
    void Bitwise_and_exp();
    void Equal_exp();
    void Compare_exp();
    void Bitwise_shift_exp();
    void Addition_exp();
    void Multy_exp();
    void Unary_exp();
    void Postfix_exp();
    void Bracket_exp();
    void Index();
    void Function_call();
    Type Function_var();
    void Array();
    void PushArray();

    class ErrorInCode: public std::exception {
    public:
        const char* what() const noexcept;
        ErrorInCode(const Lexeme &lex, std::string text = "");
    private:
        std::string text_err;
    };
private:
    void NextLex(int next_cnt = 1);
    LexicalAnalyzer lexer;
    Lexeme cur_lexeme;
    SemanticAnalyzer semantic;
    Poliz poliz;
};