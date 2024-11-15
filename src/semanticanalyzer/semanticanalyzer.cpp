#include "semanticanalyzer.hpp"

void SemanticAnalyzer::TID::PushId(const Lexeme &lex, Type type) {
    if (identifiers.check(lex.GetContent())) {
        throw std::exception();
    }
    identifiers.insert(lex.GetContent(), type);
}

Type SemanticAnalyzer::TID::CheckId(const Lexeme &lex) {
    if (!identifiers.check(lex.GetContent())) {
        throw std::exception();
    }
    return identifiers.get(lex.GetContent());
}

bool SemanticAnalyzer::TID::Check(const Lexeme &lex) {
    return identifiers.check(lex.GetContent());
}

SemanticAnalyzer::SemanticAnalyzer(): root(new Node), cur_scope(root) {}

void SemanticAnalyzer::PushId(const Lexeme &lex, Type type) {
    cur_scope->data.PushId(lex, type);
}

Type SemanticAnalyzer::CheckId(const Lexeme &lex) {
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

