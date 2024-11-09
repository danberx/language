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
        Programm();
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
    cur_lexeme = lexer.GetLex();
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
        next = nextin;
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
    if (next.GetType() != LexemeType::Bracket || next.GetContent() != ")") {
        Params();
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
    }
    Block();
}

void SyntacticAnalyzer::Expression() {
    Assignment_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Comma) {
        cur_lexeme = lexer.GetLex();
        Assignment_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Assignment_exp() {
    Lexeme next = lexer.PeekLex();
    Lexeme sign = lexer.PeekLex(2);
    if (next.GetType() == LexemeType::Identifier && sign.AssignmentOp()) {
        cur_lexeme = lexer.GetLex();
        cur_lexeme = lexer.GetLex();
    }
    Logical_or_exp();
}

void SyntacticAnalyzer::Logical_or_exp() {
    Logical_and_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Operation && next.GetContent() == "||") {
        cur_lexeme = lexer.GetLex();
        Logical_and_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Logical_and_exp() {
    Bitwise_or_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Operation && next.GetContent() == "&&") {
        cur_lexeme = lexer.GetLex();
        Bitwise_or_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_or_exp() {
    Bitwise_xor_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Operation && next.GetContent() == "|") {
        cur_lexeme = lexer.GetLex();
        Bitwise_xor_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_xor_exp() {
    Bitwise_and_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Operation && next.GetContent() == "^") {
        cur_lexeme = lexer.GetLex();
        Bitwise_and_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_and_exp() {
    Equal_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Operation && next.GetContent() == "&") {
        cur_lexeme = lexer.GetLex();
        Equal_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Equal_exp() {
    Compare_exp();
    Lexeme next = lexer.PeekLex();
    if (next.EqualOp()) {
        cur_lexeme = lexer.GetLex();
        Compare_exp();
    }
}

void SyntacticAnalyzer::Compare_exp() {
    Bitwise_shift_exp();
    Lexeme next = lexer.PeekLex();
    if (next.CompareOp()) {
        cur_lexeme = lexer.GetLex();
        Bitwise_shift_exp();
    }
}

void SyntacticAnalyzer::Bitwise_shift_exp() {
    Addition_exp();
    Lexeme next = lexer.PeekLex();
    while (next.BitwiseShiftOp()) {
        cur_lexeme = lexer.GetLex();
        Addition_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Addition_exp() {
    Multy_exp();
    Lexeme next = lexer.PeekLex();
    while (next.AdditionOp()){
        cur_lexeme = lexer.GetLex();
        Multy_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Multy_exp() {
    Unary_exp();
    Lexeme next = lexer.PeekLex();
    while (next.MultyOp()) {
        cur_lexeme = lexer.GetLex();
        Unary_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Unary_exp() {
    Lexeme next = lexer.GetLex();
    if (next.UnaryOp()) {
        cur_lexeme = lexer.GetLex();
        Unary_exp();
    }
    Postfix_exp();
}

void SyntacticAnalyzer::Postfix_exp() {
    Bracket_exp();
    Lexeme next = lexer.PeekLex();
    if (next.PostfixOp()) {
        cur_lexeme = lexer.GetLex();
    }
}

void SyntacticAnalyzer::Bracket_exp() {
   Lexeme next = lexer.PeekLex();
   if (next.GetType() == LexemeType::Literal) {
       cur_lexeme = lexer.GetLex();
       return;
   }
   if (next.GetType() == LexemeType::Bracket && next.GetContent() == "(") {
       cur_lexeme = lexer.GetLex();
       Expression();
       cur_lexeme = lexer.GetLex();
       if (cur_lexeme.GetType() != LexemeType::Bracket || cur_lexeme.GetContent() != ")") {
           // throw cur_lexeme;
       }
       return;
   }
   Lexeme next2 = lexer.PeekLex(2);
   if (next2.GetType() == LexemeType::Bracket && next2.GetContent() == "(") {
       Function_call();
   } else if (next2.GetType() == LexemeType::Bracket && next2.GetContent() == "[") {
       Index();
   } else {
       cur_lexeme = lexer.GetLex();
       if (cur_lexeme.GetType() != LexemeType::Identifier) {
           // throw cur_lexeme;
       }
   }
}

void SyntacticAnalyzer::Index() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Bracket || cur_lexeme.GetContent() != "[") {
        // throw cur_lexeme;
    }
    Expression();
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Bracket || cur_lexeme.GetContent() != "]") {
        // throw cur_lexeme;
    }
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Bracket && next.GetContent() == "[") {
        cur_lexeme = lexer.GetLex();
        Expression();
        cur_lexeme = lexer.GetLex();
        if (cur_lexeme.GetType() != LexemeType::Bracket || cur_lexeme.GetContent() != "]") {
            // throw cur_lexeme;
        }
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Function_call() {
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        // throw cur_lexeme;
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Bracket || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
    }
    Lexeme next = lexer.PeekLex();
    if (next.GetType() == LexemeType::Bracket && next.GetContent() == ")") {
        cur_lexeme = lexer.GetLex();
        return;
    }
    Expression();
    next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Comma) {
        cur_lexeme = lexer.GetLex();
        Expression();
        next = lexer.PeekLex();
    }
    cur_lexeme = lexer.GetLex();
    if (cur_lexeme.GetType() != LexemeType::Bracket || cur_lexeme.GetContent() != ")") {
       //throw cur_lexeme;
    }
}