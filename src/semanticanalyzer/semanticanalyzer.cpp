#include "semanticanalyzer.hpp"

void SemanticAnalyzer::TID::PushId(const Lexeme &lex, const std::string& type) {
    if (identifiers.count(lex.GetContent())) {
        throw std::exception();
    }
    identifiers[lex.GetContent()] = type;
}

const std::string &SemanticAnalyzer::TID::CheckId(const Lexeme &lex) {
    if (!identifiers.count(lex.GetContent())) {
        throw std::exception();
    }
    return identifiers[lex.GetContent()];
}

bool SemanticAnalyzer::TID::Check(const Lexeme &lex) {
    return identifiers.count(lex.GetContent());
}

SemanticAnalyzer::SemanticAnalyzer(): root(new Node), cur_scope(root) {}

void SemanticAnalyzer::PushId(const Lexeme &lex, const std::string &type) {
    cur_scope->data.PushId(lex, type);
}

const std::string &SemanticAnalyzer::CheckId(const Lexeme &lex) {
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