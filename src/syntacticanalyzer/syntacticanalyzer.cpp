#include "syntacticanalyzer.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3):
    lexer(path1, path2, path3), cur_lexeme("NONE", LexemeType::Other, -1) {
    try {
        Programm();
        std::cout << "OK!";
    }
    catch(ErrorInCode& err) {

        std::cout << err.what();
    }

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
        throw ErrorInCode(cur_lexeme);
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
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        throw ErrorInCode(cur_lexeme);
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
            throw ErrorInCode(cur_lexeme);
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
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Function() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "function") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsFunctionType()) {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Lexeme next = lexer.PeekLex();
    if (!next.IsBracket() || next.GetContent() != ")") {
        Params();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    Block();
}

bool SyntacticAnalyzer::Function_type() {
    return (Type() || cur_lexeme.GetType() == LexemeType::ServiceWord && cur_lexeme.GetContent() == "void");
}

void SyntacticAnalyzer::Return() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "return") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Params() {
    Function_var();
    Lexeme next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex();
        Function_var();
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Input() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "input") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Output() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "output") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Break() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "break") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Continue() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "continue") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::If() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() !=  "if") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    Block();
    Lexeme next = lexer.PeekLex();
    while (next.IsServiceWord() && next.GetContent() == "elseif") {
        NextLex(2);
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
            throw ErrorInCode(cur_lexeme);
        }
        Expression();
        NextLex();
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
            throw ErrorInCode(cur_lexeme);
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
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);

    }
    Block();
}

void SyntacticAnalyzer::For() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "for") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);

    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);

    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);

    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);

    }
    Block();
}

void SyntacticAnalyzer::Switch() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "switch") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "{") {
        throw ErrorInCode(cur_lexeme);

    }
    Cases();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "}") {
        throw ErrorInCode(cur_lexeme);

    }
}

void SyntacticAnalyzer::Cases() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "case") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsLiteral()) {
        throw ErrorInCode(cur_lexeme);

    }
    Block();
    Lexeme next = lexer.PeekLex();
    while (next.IsServiceWord() && next.GetContent() == "case") {
        NextLex();
        NextLex();
        if (!cur_lexeme.IsLiteral()) {
            throw ErrorInCode(cur_lexeme);

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
    else if (next.IsPunctuation() && next.GetContent() == ";") {
        NextLex();
        return;
    }
    else {
        Expression();
        NextLex();
        if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
            throw ErrorInCode(cur_lexeme);

        }
    }
}

void SyntacticAnalyzer::Block() {
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "{") {
        throw ErrorInCode(cur_lexeme);

    }
    Lexeme next = lexer.PeekLex();
    while (!next.IsPunctuation() || next.GetContent() != "}") {
        Command();
        next = lexer.PeekLex();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "}") {
        throw ErrorInCode(cur_lexeme);

    }
}

void SyntacticAnalyzer::Main() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "main") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);

    }
    Lexeme next = lexer.PeekLex();
    if (!next.IsBracket() || next.GetContent() != ")") {
        Params();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);

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
   if (next.IsLiteral() || next.GetType() == LexemeType::StringLiteral) {
       NextLex();
       return;
   }
   if (next.IsBracket() && next.GetContent() == "(") {
       NextLex();
       Expression();
       NextLex();
       if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
           throw ErrorInCode(cur_lexeme);

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
   throw ErrorInCode(cur_lexeme);
}

void SyntacticAnalyzer::Index() {
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "[") {
        throw ErrorInCode(cur_lexeme);

    }
    Expression();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "]") {
        throw ErrorInCode(cur_lexeme);

    }
    Lexeme next = lexer.PeekLex();
    while (next.IsBracket() && next.GetContent() == "[") {
        NextLex();
        Expression();
        NextLex();
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "]") {
            throw ErrorInCode(cur_lexeme);

        }
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Function_call() {
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);

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
       throw ErrorInCode(cur_lexeme);

    }
}

void SyntacticAnalyzer::Function_var() {
    NextLex();
    if (!cur_lexeme.IsType()) {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::NextLex(int next_cnt) {
    while (next_cnt--)
        cur_lexeme = lexer.GetLex();
}

const char* SyntacticAnalyzer::ErrorInCode::what() const noexcept {
    return text_err.c_str();
}

SyntacticAnalyzer::ErrorInCode::ErrorInCode(const Lexeme &lex, std::string text):
    text_err("Error in line " + std::to_string(lex.GetLine()) + "; On the lexeme " + lex.GetContent() + "; " + text) {}