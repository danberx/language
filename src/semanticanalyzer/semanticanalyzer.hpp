#pragma once
#include <vector>
#include <map>
#include "lexeme.hpp"
#include "borsem.hpp"

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    void PushId(const Lexeme& lex, Type type);
    Type CheckId(const Lexeme& lex);
    void CreateScope();
    void ExitScope();
    void PushFunc(std::string name, std::vector<std::pair<Type, const Lexeme&>> args);
    bool CheckFunc(std::string name, std::vector<Type> args);

private:
    class TID {
    public:
        void PushId(const Lexeme& lex, Type type);
        Type CheckId(const Lexeme& lex);
        bool Check(const Lexeme& lex);
    private:
        BorSem identifiers;
    };

    struct Node {
        std::vector<Node*> next;
        Node* prev;
        TID data;
        Node(): prev(nullptr) {}
    };

    Node * root, * cur_scope;
    BorSem functions;
};
