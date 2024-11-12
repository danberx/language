#pragma once
#include <vector>
#include <map>
#include "lexeme.hpp"

class SemanticAnalyzer {
public:
    class TID {
    public:
        void PushId(const Lexeme& lex, const std::string& type);
        const std::string& CheckId(const Lexeme& lex);
        bool Check(const Lexeme& lex);
    private:
        std::map<std::string, std::string> identifiers;
    };

    struct Node {
        std::vector<Node*> next;
        Node* prev;
        TID data;
        Node(): prev(nullptr) {}
    };

    SemanticAnalyzer();
    void PushId(const Lexeme& lex, const std::string& type);
    const std::string& CheckId(const Lexeme& lex);
    void CreateScope();
    void ExitScope();

private:
    Node * root, * cur_scope;
};
