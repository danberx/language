#include "syntacticanalyzer.hpp"

Type GetTypeFromLexeme(const Lexeme& lex) {
    if (lex.GetType() == LexemeType::ServiceWord){
        if (lex.GetContent() == "int") {
            return Type::Int;
        }
        if (lex.GetContent() == "void") {
            return Type::Void;
        }
        if (lex.GetContent() == "double") {
            return Type::Double;
        }
        if (lex.GetContent() == "string") {
            return Type::String;
        }
        if (lex.GetContent() == "intArray") {
            return Type::IntArray;
        }
        if (lex.GetContent() == "boolArray") {
            return Type::BoolArray;
        }
        if (lex.GetContent() == "charArray") {
            return Type::CharArray;
        }
        if (lex.GetContent() == "doubleArray") {
            return Type::DoubleArray;
        }
        if (lex.GetContent() == "char") {
            return Type::Char;
        }
        if (lex.GetContent() == "bool") {
            return Type::Bool;
        } else {
            throw SemanticAnalyzer::SemanticError(lex);
        }
    } else {
        if (lex.GetType() == LexemeType::Literal) return Type::Int;
        else if(lex.GetType() == LexemeType::StringLiteral) {
            if (lex.GetContent().size() == 1) {
                return Type::Char;
            } else return Type::String;
        }
        else if (lex.GetType() == LexemeType::DoubleLiteral) return Type::Double;
        else {
            throw SemanticAnalyzer::SemanticError(lex);
        }
    }
}

SyntacticAnalyzer::SyntacticAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3):
    lexer(path1, path2, path3), cur_lexeme("NONE", LexemeType::Other, -1) {
    try {
        Programm();
        std::cout << "OK!\n";
        poliz.PrintPoliz();
    }
    catch(ErrorInCode& err) {
        std::cout << "SYNTAX ERROR\n";
        std::cout << err.what();
    }
    catch(SemanticAnalyzer::SemanticError& err) {
        std::cout << "SEMANTIC ERROR\n";
        std::cout << err.what();
    }
    catch(std::string& err) {
        std::cout << err;
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

Type SyntacticAnalyzer::Var() {
    NextLex();
    if (!cur_lexeme.IsType()) {
        throw ErrorInCode(cur_lexeme);
    }
    Type cur_type = GetTypeFromLexeme(cur_lexeme);
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        throw ErrorInCode(cur_lexeme);
    }
    Lexeme name = cur_lexeme;
    Lexeme next = lexer.PeekLex();
    if (next.IsOperation() && next.GetContent() == "=") {
        NextLex();
        poliz.PushOperand(name, true);
        Lexeme operation = cur_lexeme;
        Assignment_exp();
        Type exp_type = semantic.GetLastType();
        if (cur_type == Type::Int || cur_type == Type::Double || cur_type == Type::Bool) {
            if (exp_type != Type::Int && exp_type != Type::Bool && exp_type != Type::Double) {
                throw SemanticAnalyzer::SemanticError(cur_lexeme, "Types are not matching.");
            }
        }
        else if (cur_type != exp_type) {
            throw SemanticAnalyzer::SemanticError(cur_lexeme, "Types are not matching.");
        }
        poliz.PushOperation(operation);
    }
    semantic.PushId(name, cur_type);
    return cur_type;
}

void SyntacticAnalyzer::Vars() {
    Type cur_type = Var();
    Lexeme next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex(2);
        if (!cur_lexeme.IsIdentifier()) {
            throw ErrorInCode(cur_lexeme);
        }
        Lexeme name = cur_lexeme;
        Lexeme nextin = lexer.PeekLex();
        if (nextin.IsOperation() && nextin.GetContent() == "=") {
            NextLex();
            Lexeme operation = cur_lexeme;
            poliz.PushOperand(name, true);
            Assignment_exp();
            Type exp_type = semantic.GetLastType();
            if (cur_type == Type::Int || cur_type == Type::Double || cur_type == Type::Bool) {
                if (exp_type != Type::Int && exp_type != Type::Bool && exp_type != Type::Double) {
                    throw SemanticAnalyzer::SemanticError(cur_lexeme, "Types are not matching.");
                }
            } else if (cur_type == Type::String && exp_type != Type::Char && exp_type != Type::String) {
                throw SemanticAnalyzer::SemanticError(cur_lexeme, "Types are not matching.");
            }
            else if (cur_type != exp_type) {
                throw SemanticAnalyzer::SemanticError(cur_lexeme, "Types are not matching.");
            }
            poliz.PushOperation(operation);
        }
        semantic.PushId(name, cur_type);
        next = lexer.PeekLex();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Array() {
    NextLex();
    if (!cur_lexeme.IsArrayType()) {
        throw ErrorInCode(cur_lexeme);
    }
    Type type = GetTypeFromLexeme(cur_lexeme);
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.PushId(cur_lexeme, type);
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    Type exp_type = semantic.GetLastType();
    if (exp_type != Type::Int && exp_type != Type::Double && exp_type != Type::Bool) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Array size must be Int (or Bool or Double)");
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::PushArray() {
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::ServiceWord || cur_lexeme.GetContent() != "push") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        throw ErrorInCode(cur_lexeme);
    }
    Type array_type = semantic.CheckId(cur_lexeme);
    poliz.PushOperand(cur_lexeme, true);
    if (array_type != Type::DoubleArray && array_type != Type::IntArray && array_type != Type::CharArray && array_type != Type::BoolArray) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Not array type");
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    Type add_type = semantic.GetLastType();
    if (array_type == Type::CharArray) {
        if (add_type != Type::Char) {
            throw SemanticAnalyzer::SemanticError(cur_lexeme, "types are not matching");
        }
    }
    else if (add_type != Type::Int && add_type != Type::Double && add_type != Type::Bool) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "types are not matching");
    }
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Punctuation || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    poliz.PushPushArray();
}

void SyntacticAnalyzer::Function() {
    semantic.CreateScope();
    NextLex();
    std::string name;
    std::vector<Type> args;
    Type return_type;
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "function") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsFunctionType()) {
        throw ErrorInCode(cur_lexeme);
    }
    return_type = GetTypeFromLexeme(cur_lexeme);
    semantic.SetType(return_type);
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);
    }
    name = cur_lexeme.GetContent();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Lexeme next = lexer.PeekLex();
    if (!next.IsBracket() || next.GetContent() != ")") {
        Params(args);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.PushFunc(name, return_type, args, cur_lexeme, poliz.GetCur());
    Block();
    if (!semantic.GetReturn()) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Function must have return");
    }
    semantic.SetReturn(0);
    semantic.ExitScope();
}

void SyntacticAnalyzer::Return() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "return") {
        throw ErrorInCode(cur_lexeme);
    }
    Lexeme next = lexer.PeekLex();
    if (next.IsPunctuation() && next.GetContent() == ";") {
        if (semantic.GetReturnType() == Type::Void) {
            semantic.SetReturn(1);
            NextLex();
            return;
        }
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "return must be the same type as the function");
    }
    Expression();
    Type cur_type = semantic.GetLastType();
    Type return_type = semantic.GetReturnType();
    if (cur_type == Type::Int || cur_type == Type::Bool || cur_type == Type::Double) {
        if (return_type != Type::Int && return_type != Type::Bool && return_type != Type::Double) {
            throw SemanticAnalyzer::SemanticError(cur_lexeme, "return must be the same type as the function");
        }
    } else if (cur_type != return_type) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "return must be the same type as the function");
    }
    semantic.SetReturn(1);
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
}

void SyntacticAnalyzer::Params(std::vector<Type>& args) {
    Type t = Function_var();
    args.push_back(t);
    Lexeme next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex();
        Type type = Function_var();
        args.push_back(type);
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
    semantic.CheckId(cur_lexeme);
    poliz.PushOperand(cur_lexeme, true);
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
    poliz.PushInput();
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
    semantic.ClearSemStack();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
    poliz.PushOutput();
}

void SyntacticAnalyzer::Break() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "break") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.CheckCycle(cur_lexeme);
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
    set_breaks.top().push_back(poliz.GetCur());
    poliz.Skip();
}

void SyntacticAnalyzer::Continue() {
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "continue") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.CheckCycle(cur_lexeme);
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
    set_continues.top().push_back(poliz.GetCur());
    poliz.Skip();
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
    int false_pointer = poliz.GetCur();
    poliz.Skip();
    NextLex();
    semantic.CheckBool(cur_lexeme);
    semantic.ClearSemStack();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.CreateScope();
    std::vector<int> set_adresses;
    Block();
    set_adresses.push_back(poliz.GetCur());
    poliz.Skip();
    semantic.ExitScope();
    Lexeme next = lexer.PeekLex();
    while (next.IsServiceWord() && next.GetContent() == "elseif") {
        NextLex(2);
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
            throw ErrorInCode(cur_lexeme);
        }
        poliz.SetGoTo(false_pointer, poliz.GetCur(), true);

        Expression();
        false_pointer = poliz.GetCur();
        poliz.Skip();
        NextLex();
        semantic.CheckBool(cur_lexeme);
        semantic.ClearSemStack();
        if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
            throw ErrorInCode(cur_lexeme);
        }
        semantic.CreateScope();
        Block();
        set_adresses.push_back(poliz.GetCur());
        poliz.Skip();
        semantic.ExitScope();
        next = lexer.PeekLex();
    }
    poliz.SetGoTo(false_pointer, poliz.GetCur(), true);
    if (next.IsServiceWord() && next.GetContent() == "else") {
        NextLex();
        semantic.CreateScope();
        Block();
        semantic.ExitScope();
    }
    int adress = poliz.GetCur();
    for (int i = 0; i < set_adresses.size(); ++i) {
        poliz.SetGoTo(set_adresses[i], adress, false);
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
    int start_adress = poliz.GetCur();
    Expression();
    int false_pointer = poliz.GetCur();
    poliz.Skip();
    NextLex();
    semantic.CheckBool(cur_lexeme);
    semantic.ClearSemStack();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);

    }
    semantic.CreateScope();
    semantic.EnterCycle();
    set_breaks.push({});
    set_continues.push({});
    Block();
    poliz.Skip();
    poliz.SetGoTo(poliz.GetCur() - 1, start_adress, false);
    int end_adress = poliz.GetCur();
    poliz.SetGoTo(false_pointer, end_adress, true);
    for (auto el: set_breaks.top()) {
        poliz.SetGoTo(el, end_adress, false);
    }
    for (auto el: set_continues.top()) {
        poliz.SetGoTo(el, start_adress, false);
    }
    semantic.ExitSycle();
    semantic.ExitScope();
    set_continues.pop();
    set_breaks.pop();
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
    int condition_adress = poliz.GetCur();
    Expression();
    int false_pointer = poliz.GetCur();
    poliz.Skip();
    int true_pointer = poliz.GetCur();
    poliz.Skip();
    NextLex();
    semantic.CheckBool(cur_lexeme);
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != ";") {
        throw ErrorInCode(cur_lexeme);
    }
    int go_adress = poliz.GetCur();
    Expression();
    poliz.Skip();
    poliz.SetGoTo(poliz.GetCur() - 1, condition_adress, false);

    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.CreateScope();
    semantic.EnterCycle();
    poliz.SetGoTo(true_pointer, poliz.GetCur(), false);
    set_breaks.push({});
    set_continues.push({});
    Block();
    poliz.Skip();
    poliz.SetGoTo(poliz.GetCur() - 1, go_adress, false);
    int end_adress = poliz.GetCur();
    poliz.SetGoTo(false_pointer, end_adress, true);
    semantic.ExitSycle();
    semantic.ExitScope();
    for (auto el: set_breaks.top()) {
        poliz.SetGoTo(el, end_adress, false);
    }
    for (auto el: set_continues.top()) {
        poliz.SetGoTo(el, go_adress, false);
    }
    set_continues.pop();
    set_breaks.pop();
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
    Type type = semantic.CheckId(cur_lexeme);
    if (type != Type::Int) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Switch type must be integer");
    }
    switch_identifiers.push(cur_lexeme);
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    semantic.CreateScope();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "{") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.EnterSwitch();
    Cases();
    switch_identifiers.pop();
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "}") {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.ExitScope();
    semantic.ExitSwitch();
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
    Type cur_type = GetTypeFromLexeme(cur_lexeme);
    if (cur_type != Type::Int) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Case type must be integer");
    }
    semantic.InsertSwitchCase(std::stoi(cur_lexeme.GetContent()), cur_lexeme);
    poliz.PushOperand(switch_identifiers.top(), 1);
    poliz.PushOperand(cur_lexeme, false);
    Lexeme cmp("==", LexemeType::Operation, cur_lexeme.GetLine());
    poliz.PushOperation(cmp);
    int false_pointer = poliz.GetCur();
    poliz.Skip();

    semantic.CreateScope();
    std::vector<int> adresses;
    Block();
    adresses.push_back(poliz.GetCur());
    poliz.Skip();
    semantic.ExitScope();
    Lexeme next = lexer.PeekLex();
    while (next.IsServiceWord() && next.GetContent() == "case") {
        NextLex();
        NextLex();
        if (!cur_lexeme.IsLiteral()) {
            throw ErrorInCode(cur_lexeme);
        }
        cur_type = GetTypeFromLexeme(cur_lexeme);
        if (cur_type != Type::Int) {
            throw SemanticAnalyzer::SemanticError(cur_lexeme, "Case type must be integer");
        }
        poliz.SetGoTo(false_pointer, poliz.GetCur(), true);
        semantic.InsertSwitchCase(std::stoi(cur_lexeme.GetContent()), cur_lexeme);
        poliz.PushOperand(switch_identifiers.top(), 1);
        poliz.PushOperand(cur_lexeme, false);
        poliz.PushOperation(cmp);
        false_pointer = poliz.GetCur();
        poliz.Skip();
        semantic.CreateScope();
        Block();
        adresses.push_back(poliz.GetCur());
        poliz.Skip();
        semantic.ExitScope();
        next = lexer.PeekLex();
    }
    for (auto el: adresses) {
        poliz.SetGoTo(el, poliz.GetCur(), false);
    }
    poliz.SetGoTo(false_pointer, poliz.GetCur(), true);
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
        } else if (next.GetContent() == "push") {
            PushArray();
        }
        else if (next.IsType()) {
            Vars();
        }
        else if (next.IsArrayType()) {
            Array();
        }
        else {
            throw ErrorInCode(next);
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
        if (next.IsEnd() ) {
            throw ErrorInCode(cur_lexeme);
        }
        Command();
        next = lexer.PeekLex();
    }
    NextLex();
    if (!cur_lexeme.IsPunctuation() || cur_lexeme.GetContent() != "}") {
        throw ErrorInCode(cur_lexeme);

    }
}

void SyntacticAnalyzer::Main() {
    semantic.CreateScope();
    semantic.SetType(Type::Int);
    NextLex();
    if (!cur_lexeme.IsServiceWord() || cur_lexeme.GetContent() != "main") {
        throw ErrorInCode(cur_lexeme);
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);

    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
        throw ErrorInCode(cur_lexeme);
    }
    start_programm = poliz.GetCur();
    Block();
    semantic.ExitScope();
}

void SyntacticAnalyzer::Expression() {
    Assignment_exp();
    Lexeme next = lexer.PeekLex();
    while (next.GetType() == LexemeType::Comma) {
        cur_lexeme = lexer.GetLex();
        Lexeme operation = cur_lexeme;
        Assignment_exp();
        next = lexer.PeekLex();
        poliz.PushOperation(operation);
    }
}

void SyntacticAnalyzer::Assignment_exp() {
    Logical_or_exp();
    Lexeme next = lexer.PeekLex();
    while (next.AssignmentOp()) {
        NextLex();
        semantic.PushSemStack(cur_lexeme);
        Lexeme operation = cur_lexeme;
        Logical_or_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Logical_or_exp() {
    Logical_and_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "||") {
        NextLex();
        semantic.PushSemStack(cur_lexeme);
        Lexeme operation = cur_lexeme;
        Logical_and_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Logical_and_exp() {
    Bitwise_or_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "&&") {
        NextLex();
        semantic.PushSemStack(cur_lexeme);
        Lexeme operation = cur_lexeme;
        Bitwise_or_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_or_exp() {
    Bitwise_xor_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "|") {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Bitwise_xor_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_xor_exp() {
    Bitwise_and_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "^") {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack((cur_lexeme));
        Bitwise_and_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_and_exp() {
    Equal_exp();
    Lexeme next = lexer.PeekLex();
    while (next.IsOperation() && next.GetContent() == "&") {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Equal_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Equal_exp() {
    Compare_exp();
    Lexeme next = lexer.PeekLex();
    while (next.EqualOp()) {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Compare_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Compare_exp() {
    Bitwise_shift_exp();
    Lexeme next = lexer.PeekLex();
    while (next.CompareOp()) {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Bitwise_shift_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Bitwise_shift_exp() {
    Addition_exp();
    Lexeme next = lexer.PeekLex();
    while (next.BitwiseShiftOp()) {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Addition_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Addition_exp() {
    Multy_exp();
    Lexeme next = lexer.PeekLex();
    while (next.AdditionOp()){
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Multy_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Multy_exp() {
    Unary_exp();
    Lexeme next = lexer.PeekLex();
    while (next.MultyOp()) {
        NextLex();
        Lexeme operation = cur_lexeme;
        semantic.PushSemStack(cur_lexeme);
        Unary_exp();
        semantic.CheckBin();
        poliz.PushOperation(operation);
        next = lexer.PeekLex();
    }
}

void SyntacticAnalyzer::Unary_exp() {
    Lexeme next = lexer.PeekLex();
    if (next.UnaryOp()) {
        NextLex();
        Lexeme operation = cur_lexeme;
        operation.SetContent(operation.GetContent() + "prefix");
        semantic.PushSemStack(cur_lexeme);
        Unary_exp();
        semantic.CheckUno();
        poliz.PushOperation(operation);
        return;
    }
    Postfix_exp();
}

void SyntacticAnalyzer::Postfix_exp() {
    Bracket_exp();
    Lexeme next = lexer.PeekLex();
    if (next.PostfixOp()) {
        NextLex();
        Lexeme operation = cur_lexeme;
        operation.SetContent(cur_lexeme.GetContent() + "postfix");
        semantic.PushSemStack(cur_lexeme);
        semantic.CheckPostfix();
        poliz.PushOperation(operation);
    }
}

void SyntacticAnalyzer::Bracket_exp() {
   Lexeme next = lexer.PeekLex();
   if (next.IsLiteral()) {
       NextLex();
       semantic.PushSemStack(cur_lexeme);
       poliz.PushOperand(cur_lexeme, false);
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
           semantic.PushSemStack(cur_lexeme);
           poliz.PushOperand(cur_lexeme, true);
       }
       return;
   }
   throw ErrorInCode(next);
}

void SyntacticAnalyzer::Index() {
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);
    }
    Type type = semantic.CheckId(cur_lexeme);
    if (type != Type::IntArray && type != Type::BoolArray && type != Type::CharArray && type != Type::DoubleArray && type != Type::String) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Not array type");
    }
    poliz.PushOperand(cur_lexeme, true);
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "[") {
        throw ErrorInCode(cur_lexeme);
    }
    Expression();
    Type index_type = semantic.GetLastType();
    if (index_type != Type::Int && index_type != Type::Bool && index_type != Type::Double) {
        throw SemanticAnalyzer::SemanticError(cur_lexeme, "Array index must be Int (or Bool or Double)");
    }
    semantic.PopSemStack();
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "]") {
        throw ErrorInCode(cur_lexeme);
    }
    Type array_type;
    if (type == Type::IntArray) {
        array_type = Type::Int;
    } else if (type == Type::DoubleArray) {
        array_type = Type::Double;
    } else if (type == Type::BoolArray) {
        array_type = Type::Bool;
    } else {
        array_type = Type::Char;
    }
    semantic.PushSemStack(array_type);
    poliz.PushIndex();
}

void SyntacticAnalyzer::Function_call() {
    NextLex();
    if (!cur_lexeme.IsIdentifier()) {
        throw ErrorInCode(cur_lexeme);
    }
    Lexeme func = cur_lexeme;
    std::string name = cur_lexeme.GetContent();
    std::vector<Type> fact_args;
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != "(") {
        throw ErrorInCode(cur_lexeme);
    }
    Lexeme next = lexer.PeekLex();
    if (next.IsBracket() && next.GetContent() == ")") {
        NextLex();
        semantic.CheckFun(name, fact_args, cur_lexeme);
        semantic.PushSemStack(semantic.GetFunctionType(name));
        return;
    }
    Assignment_exp();
    fact_args.push_back(semantic.GetLastType());
    semantic.PopSemStack();
    next = lexer.PeekLex();
    while (next.IsComma()) {
        NextLex();
        Assignment_exp();
        fact_args.push_back(semantic.GetLastType());
        next = lexer.PeekLex();
        semantic.PopSemStack();
    }
    NextLex();
    if (!cur_lexeme.IsBracket() || cur_lexeme.GetContent() != ")") {
       throw ErrorInCode(cur_lexeme);
    }
    semantic.CheckFun(name, fact_args, cur_lexeme);
    semantic.PushSemStack(semantic.GetFunctionType(name));
    poliz.PushFunctionCall(func);
}

Type SyntacticAnalyzer::Function_var() {
    NextLex();
    if (!cur_lexeme.IsType()) {
        throw ErrorInCode(cur_lexeme);
    }
    Type cur = GetTypeFromLexeme(cur_lexeme);
    NextLex();
    if (cur_lexeme.GetType() != LexemeType::Identifier) {
        throw ErrorInCode(cur_lexeme);
    }
    semantic.PushId(cur_lexeme, cur);
    return cur;
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