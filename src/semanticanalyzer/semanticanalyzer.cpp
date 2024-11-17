#include "semanticanalyzer.hpp"

void SemanticAnalyzer::TID::PushId(const Lexeme &lex, Type type) {
    if (identifiers.check(lex.GetContent())) {
        throw std::exception();
    }
    identifiers.insert(lex.GetContent(), type);
}

Type SemanticAnalyzer::TID::CheckId(const Lexeme &lex) const {
    if (!identifiers.check(lex.GetContent())) {
        throw std::exception();
    }
    return identifiers.get(lex.GetContent());
}

bool SemanticAnalyzer::TID::Check(const Lexeme &lex) const {
    return identifiers.check(lex.GetContent());
}

SemanticAnalyzer::SemanticAnalyzer(): root(new Node), cur_scope(root) {}

void SemanticAnalyzer::PushId(const Lexeme &lex, Type type) {
    cur_scope->data.PushId(lex, type);
}

Type SemanticAnalyzer::CheckId(const Lexeme &lex) const {
    Node* check_scope = cur_scope;
    while (check_scope != nullptr && !check_scope->data.Check(lex)) {
        check_scope = check_scope->prev;
    }
    if (check_scope == nullptr) {
        throw std::exception();
    }
    return check_scope->data.CheckId(lex);
}

void SemanticAnalyzer::CreateScope() {
    cur_scope->next.push_back(new Node);
    cur_scope->next.back()->prev = cur_scope;
    cur_scope = cur_scope->next.back();
}

void SemanticAnalyzer::ExitScope() {
    if (cur_scope->prev == nullptr)
        throw std::exception();
    cur_scope = cur_scope->prev;
}

void SemanticAnalyzer::PushFunc(std::string name, std::vector<std::pair<Type, const Lexeme&>> args) {
    name = name + "#";
    for (auto [arg, lex] : args) {
        name += std::to_string(static_cast<int>(arg));
    }

    if (functions.check(name)) {
        throw std::exception();
    }

    functions.insert(name, Type::Function);
    CreateScope();
    for (auto [arg, lex] : args) {
        PushId(lex, arg);
    }
}

void SemanticAnalyzer::CheckFunc(std::string name, std::vector<Type> args) const {
    name = name + "#";
    for (auto arg : args) {
        name += std::to_string(static_cast<int>(arg));
    }

    if (functions.check(name)) {
        throw std::exception();
    }
}

void SemanticAnalyzer::PushSemStack(Type type) {
    semStack.emplace_back(type);
}

void SemanticAnalyzer::PushSemStack(const Lexeme &lex) {
    semStack.emplace_back(lex, *this);
}

void SemanticAnalyzer::CheckBin() {
    int sz = semStack.size();
    if (sz < 3 || !semStack[sz - 2].IsOperation() || semStack[sz - 1].IsOperation() || semStack[sz-3].IsOperation()) {
        throw std::exception();
    }

    if (semStack[sz - 2].GetLex().AssignmentOp() && semStack[sz - 1].GetType() == semStack[sz - 3].GetType() && semStack[sz - 3].IsLvalue()) {
        Type t = semStack[sz - 1].GetType();
        semStack.erase(semStack.end() - 3, semStack.end() - 1);
        semStack.emplace_back(t , 1);
    }
    else if ((semStack[sz - 2].GetLex().GetContent() == "&&" || semStack[sz - 2].GetLex().GetContent() == "||")
    && semStack[sz - 1].GetType() == Type::Bool && semStack[sz - 3].GetType() == Type::Bool) {
        semStack.erase(semStack.end() - 3, semStack.end() - 1);
        semStack.emplace_back(Type::Bool);
    }
    else if ((semStack[sz - 2].GetLex().AdditionOp() || semStack[sz - 2].GetLex().BitwiseShiftOp() || semStack[sz - 2].GetLex().MultyOp()
    || semStack[sz - 2].GetLex().GetContent() == "&" || semStack[sz - 2].GetLex().GetContent() == "|" || semStack[sz - 2].GetLex().GetContent() == "^")
    && semStack[sz - 1].GetType() == Type::Int && semStack[sz - 3].GetType() == Type::Int) {
        semStack.erase(semStack.end() - 3, semStack.end() - 1);
        semStack.emplace_back(Type::Int);
    }
    else if ((semStack[sz - 2].GetLex().EqualOp() || semStack[sz - 2].GetLex().CompareOp()) &&
    semStack[sz - 1].GetType() == semStack[sz - 3].GetType()) {
        semStack.erase(semStack.end() - 3, semStack.end() - 1);
        semStack.emplace_back(Type::Bool);
    }
    else {
        throw std::exception();
    }
}

void SemanticAnalyzer::CheckUno() {
    int sz = semStack.size();
    if (sz < 2 || semStack[sz - 1].IsOperation() || !semStack[sz - 2].IsOperation()) {
        throw std::exception();
    }
    if (semStack[sz - 2].GetLex().UnaryOp() && semStack[sz - 2].GetLex().GetContent() != "!"
    && semStack[sz - 1].GetType() == Type::Int) {

    }
}

SemanticAnalyzer::NodeSemStack::NodeSemStack(Type t, bool is_lvalue_): type(t), is_operation(0), is_lvalue(is_lvalue_) {}

SemanticAnalyzer::NodeSemStack::NodeSemStack(const Lexeme &lx, const SemanticAnalyzer& semanticAnalyzer) {
    if (lex.IsOperation()) {
        is_operation = 1;
        is_lvalue = 0;
        lex = lx;
    }
    else if (lex.IsIdentifier()) {
        is_operation = 0;
        is_lvalue = 1;
        type = semanticAnalyzer.CheckId(lex);
    }
    else {
        throw std::exception();
    }
}

const Lexeme& SemanticAnalyzer::NodeSemStack::GetLex() {
    return lex;
}

Type SemanticAnalyzer::NodeSemStack::GetType() const {
    return type;
}

bool SemanticAnalyzer::NodeSemStack::IsOperation() const {
    return is_operation;
}

bool SemanticAnalyzer::NodeSemStack::IsLvalue() const {
    return is_lvalue;
}