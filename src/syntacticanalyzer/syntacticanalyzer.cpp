#include "syntacticanalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3):
    lexer(path1, path2, path3), cur_lexeme("NONE", LexemeType::Other, -1) {
    Programm();
}


void SyntacticAnalyzer::Programm() {
    Lexeme next = lexer.PeekLex();
    if (next.IsServiceWord() && next.GetContent() == "function") {
        Function();
        Programm();
    }
    else if (next.IsServiceWord() && next.GetContent() == "main") {
        Main();
    }
    else if (next.IsType()) {
        Vars();
        Programm();
    }
    NextLex();
    if (cur_lexeme.GetContent() != "END") {
        //throw cur_lexeme;
        throw std::exception();
    }
}

bool SyntacticAnalyzer::Type() {
    if (cur_lexeme.GetType() != LexemeType::ServiceWord) return false;
    std::string s = cur_lexeme.GetContent();
    return (s == "int" || s == "double" || s == "string" || s == "bool" || s == "char" || s == "array");
}

void SyntacticAnalyzer::Var() {
    NextLex();
    if (!cur_lexeme.IsType()) {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        //throw cur_lexeme;
        throw std::exception();
    }
    Lexeme next = lexer.PeekLex();
    if (next.IsOperation() && next.GetContent() == "=") {
        NextLex();
        Assignment_exp();
    }
}

void SyntacticAnalyzer::Vars() {
    Var();
    Lexeme next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex(2);
        if (!cur_lexeme.IsIdentifier()) {
            //throw cur_lexeme;
            throw std::exception();
        }
        Lexeme nextin = lexer.PeekLex();
        if (nextin.IsOperation() && nextin.GetContent() == "=") {
            NextLex();
            Assignment_exp();
        }
        next = lexer.PeekLex();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Function() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "function") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsFunctionType()) {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Lexeme next = lexer.PeekLex();
    if (!next.IsBracket() || next.GetContent() != ")") {
        Params();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Block();
}

bool SyntacticAnalyzer::Function_type() {
    return (Type() || cur_lexeme.GetType() == LexemeType::ServiceWord && cur_lexeme.GetContent() == "void");
}

void SyntacticAnalyzer::Return() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "return") {
        // throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Params() {
    Var();
    Lexeme next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex();
        Var();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Input() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "input") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Output() {
    NextLex();
    if (!cur_lexeme.IsIdentifier() || cur_lexeme.GetContent() != "output") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Break() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "break") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Continue() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "continue") {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        // throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::If() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() !=  "if") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Block();
    Lexeme next = lexer.PeekLex();
    while (next.IsServiceWord() && next.GetContent() == "elseif") {
        NextLex(2);
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
            //throw cur_lexeme;
            throw std::exception();
        }
        Expression();
        NextLex();
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
            //throw cur_lexeme;
            throw std::exception();
        }
        Block();
        next = lexer.PeekLex();
    }
    if (next.IsServiceWord() && next.GetContent() == "else") {
        NextLex();
        Block();
    }
}

void SyntacticAnalyzer::While() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "while") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Block();
}

void SyntacticAnalyzer::For() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "for") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Block();
}

void SyntacticAnalyzer::Switch() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "switch") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "{") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Cases();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "}") {
        //throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Cases() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "case") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsLiteral()) {
        //throw cur_lexeme;
        throw std::exception();
    }
    Block();
    Lexeme next = lexer.PeekLex();
    while (next.IsServiceWord() && next.GetContent() == "case") {
        NextLex();
        NextLex();
        if (!cur_lexeme.IsLiteral()) {
            //throw cur_lexeme;
            throw std::exception();
        }
        Block();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Command() {
    Lexeme next = lexer.PeekLex();
    if (next.IsServiceWord()) {
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
        else if (next.IsType()) {
            Vars();
        }
    }
    else {
        Expression();
        NextLex();
        if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
            //throw cur_lexeme;
            throw std::exception();
        }
    }
}

void SyntacticAnalyzer::Block() {
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "{") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Lexeme next = lexer.PeekLex();
    while (!next.IsPunctuation() || next.GetContent() != "}") {
        Command();
        next = lexer.PeekLex();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "}") {
        //throw cur_lexeme;
        throw std::exception();
    }
}

void SyntacticAnalyzer::Main() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "main") {
        //throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        //throw cur_lexeme;
        throw std::exception();
    }
    Lexeme next = lexer.PeekLex();
    if (!next.IsBracket() || next.GetContent() != ")") {
        Params();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        //throw cur_lexeme;
        throw std::exception();
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
    Logical_or_exp();
    Lexeme next = lexer.PeekLex();
    while (next.AssignmentOp()) {
        NextLex();
        Logical_or_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Logical_or_exp() {
    Logical_and_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "||") {
        NextLex();
        Logical_and_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Logical_and_exp() {
    Bitwise_or_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "&&") {
        NextLex();
        Bitwise_or_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_or_exp() {
    Bitwise_xor_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "|") {
        NextLex();
        Bitwise_xor_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_xor_exp() {
    Bitwise_and_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "^") {
        NextLex();
        Bitwise_and_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_and_exp() {
    Equal_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "&") {
        NextLex();
        Equal_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Equal_exp() {
    Compare_exp();
    Lexeme next = lexer.PeekLex();
    while (next.EqualOp()) {
        NextLex();
        Compare_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Compare_exp() {
    Bitwise_shift_exp();
    Lexeme next = lexer.PeekLex();
    while (next.CompareOp()) {
        NextLex();
        Bitwise_shift_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_shift_exp() {
    Addition_exp();
    Lexeme next = lexer.PeekLex();
    while (next.BitwiseShiftOp()) {
        NextLex();
        Addition_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Addition_exp() {
    Multy_exp();
    Lexeme next = lexer.PeekLex();
    while (next.AdditionOp()){
        NextLex();
        Multy_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Multy_exp() {
    Unary_exp();
    Lexeme next = lexer.PeekLex();
    while (next.MultyOp()) {
        NextLex();
        Unary_exp();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Unary_exp() {
    Lexeme next = lexer.PeekLex();
    if (next.UnaryOp()) {
        NextLex();
        Unary_exp();
        return;
    }
    Postfix_exp();
}

void SyntacticAnalyzer::Postfix_exp() {
    Bracket_exp();
    Lexeme next = lexer.PeekLex();
    if (next.PostfixOp()) {
        NextLex();
    }
}

void SyntacticAnalyzer::Bracket_exp() {
   Lexeme next = lexer.PeekLex();
   if (next.IsLiteral()) {
       NextLex();
       return;
   }
   if (next.IsBracket() && next.GetContent() == "(") {
       NextLex();
       Expression();
       NextLex();
       if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
           // throw cur_lexeme;
           throw std::exception();
       }
       return;
   }
   if (next.IsIdentifier()) {
       Lexeme next2 = lexer.PeekLex(2);
       if (next2.IsBracket() && next2.GetContent() == "(") {
           Function_call();
       } else if (next2.IsBracket() && next2.GetContent() == "[") {
           Index();
       } else {
           NextLex();
       }
       return;
   }
   throw std::exception();
}

void SyntacticAnalyzer::Index() {
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "[") {
        // throw cur_lexeme;
        throw std::exception();
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "]") {
        // throw cur_lexeme;
        throw std::exception();
    }
    Lexeme next = lexer.PeekLex();
    while (next.IsBracket() && next.GetContent() == "[") {
        NextLex();
        Expression();
        NextLex();
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "]") {
            // throw cur_lexeme;
            throw std::exception();
        }
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Function_call() {
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        // throw cur_lexeme;
        throw std::exception();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        // throw cur_lexeme;
        throw std::exception();
    }
    Lexeme next = lexer.PeekLex();
    if (next.IsBracket() && next.GetContent() == ")") {
        NextLex();
        return;
    }
    Expression();
    next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex();
        Expression();
        next = lexer.PeekLex();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
       //throw cur_lexeme;
       throw std::exception();
    }
}

void SyntacticAnalyzer::NextLex(int next_cnt) {
    while (next_cnt--)
        cur_lexeme = lexer.GetLex();
}