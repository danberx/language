#pragma once

#include "../lexemeanalyzer/lexicalanalyzer.hpp"

class SyntacticAnalyzer {
    void programm();
    void type();
    //...

private:
    LexicalAnalyzer lexer;
    Lexeme cur_lexeme;
};