#include "syntacticanalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3):
    lexer(path1, path2, path3), cur_lexeme("NONE", LexemeType::Other, -1) {
    Programm();
}


void SyntacticAnalyzer::Programm() {
    Lexeme next = lexer.PeekLex();
    if (next.GetType() == LexemeType::ServiceWord && next.GetContent() == "function") {
        Function();
        Programm();
        return;
    }
    if (next.GetType() == LexemeType::ServiceWord && next.GetContent() == "main") {
        Main();
    }
    else {
        Vars();
    }
    // throw cur_lexeme;
}

bool SyntacticAnalyzer::Type() {
    if (cur_lexeme.GetType() != LexemeType::ServiceWord) return false;
    std::string s = cur_lexeme.GetContent();
    return (s == "int" || s == "double" || s == "string" || s == "bool" || s == "char" || s == "array");
}

void SyntacticAnalyzer::Var() {
    cur_lexeme = lexer.GetLex();
    if (!Type()) {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.PeekLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        //throw cur_lexeme;
    }
    Lexeme next = lexer.PeekLex();
    if (next.GetType() == LexemeType::Operation && next.GetContent() == "=") {
        cur_lexeme = lexer.GetLex();
        // Assignment_expression();
        return;
    }
}

void SyntacticAnalyzer::Vars() {
    Var();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Comma) {
        cur_lexeme = lexer.GetLex();
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Identifier) {
            //throw cur_lexeme;
        }
        Lexeme nextin = lexer.PeekLex();
        if (nextin.GetType() == LexemeType::Operation && nextin.GetContent() == "=") {
            cur_lexeme = lexer.GetLex();
            // Assignment_expression();
            nextin = lexer.PeekLex();
        }
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Function() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "function") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (!Function_type()) {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
    }
    Lexeme next = lexer.PeekLex();
    if (next.GetType() != LexemeType::Punctuation || next.GetContent() != ")") {
        Params();
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
    Block();
}

bool SyntacticAnalyzer::Function_type() {
    return (Type() || cur_lexeme.GetType() == LexemeType::ServiceWord && cur_lexeme.GetContent() == "void");
}

void SyntacticAnalyzer::Return() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "return") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Params() {
    Var();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Comma) {
        cur_lexeme = lexer.GetLex();
        Var();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Input() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "input") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Output() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "output") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Break() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "break") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Continue() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "continue") {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
    }
}

void SyntacticAnalyzer::If() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() !=  "if") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
    Block();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::ServiceWord && next.GetContent() == "elseif") {
        cur_lexeme = lexer.GetLex();
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
            //throw cur_lexeme;
        }
        Expression();
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
            //throw cur_lexeme;
        }
        Block();
        next = lexer.PeekLex();
    }
    if (next.GetType() == LexemeType::ServiceWord && next.GetContent() == "else") {
        cur_lexeme = lexer.GetLex();
        Block();
    }
}

void SyntacticAnalyzer::While() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "while") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
}

void SyntacticAnalyzer::For() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "for") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        //throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
        //throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
    Block();
}

void SyntacticAnalyzer::Switch() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "switch") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "{") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    Cases();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "}") {
        //throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Cases() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "case") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Literal) {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ":") {
        //throw cur_lexeme;
    }
    Block();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::ServiceWord && next.GetContent() == "case") {
        cur_lexeme = lexer.GetLex();
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Literal) {
            //throw cur_lexeme;
        }
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ":") {
            //throw cur_lexeme;
        }
        Block();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Command() {
    Lexeme next = lexer.PeekLex();
    if (next.GetType() == LexemeType::ServiceWord) {
        if (next.GetContent() == "if") {
            If();
        }
        else if (next.GetContent() == "while") {
            While();
        }
        else if (next.GetContent() == "for") {
            For();
        }
        else if (next.GetContent() == "break") {
            Break();
        }
        else if (next.GetContent() == "continue") {
            Continue();
        }
        else if (next.GetContent() == "input") {
            Input();
        }
        else if (next.GetContent() == "output") {
            Output();
        }
        else if (next.GetContent() == "return") {
            Return();
        }
        else if (next.GetContent() == "switch") {
            Switch();
        }
    }
    else if (next.IsType()) {
        Vars();
    }
    else {
        Expression();
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ";") {
            //throw cur_lexeme;
        }
    }
}

void SyntacticAnalyzer::Block() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "{") {
        //throw cur_lexeme;
    }
    Lexeme next = lexer.PeekLex();
    while (next.GetType() != LexemeType::Punctuation || next.GetContent() != "}") {
        Command();
        next = lexer.PeekLex();
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "}") {
        //throw cur_lexeme;
    }
}

void SyntacticAnalyzer::Main() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "main") {
        //throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
    }
    Lexeme next = lexer.PeekLex();
    if (next.GetType() != LexemeType::Punctuation || next.GetContent() != ")") {
        Params();
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
    Block();
}

void SyntacticAnalyzer::Expression() {

}

