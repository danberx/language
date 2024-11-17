#pragma once
#include <vector>
#include <map>
#include "lexeme.hpp"
#include "borsem.hpp"

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    void PushId(const Lexeme& lex, Type type);
    Type CheckId(const Lexeme& lex) const;
    void CreateScope();
    void ExitScope();
    void PushFunc(std::string name, std::vector<std::pair<Type, const Lexeme&>> args);
    void CheckFunc(std::string name, std::vector<Type> args) const;
    void PushSemStack(const Lexeme &lex);
    void PushSemStack(Type type);
    void CheckBin();
    void CheckUno();

private:
    class TID {
    public:
        void PushId(const Lexeme& lex, Type type);
        Type CheckId(const Lexeme& lex) const;
        bool Check(const Lexeme& lex) const;
    private:
        BorSem identifiers;
    };

    struct Node {
        std::vector<Node*> next;
        Node* prev;
        TID data;
        Node(): prev(nullptr) {}
    };

    class NodeSemStack {
    public:
        NodeSemStack(const Lexeme &lx, const SemanticAnalyzer& semanticAnalyzer);
        NodeSemStack(Type t, bool is_lvalue_ = 0);
        const Lexeme& GetLex();
        Type GetType() const;
        bool IsLvalue() const;
        bool IsOperation() const;
    private:
        bool is_operation, is_lvalue;
        Lexeme lex;
        Type type;
    };

    Node * root, * cur_scope;
    BorSem functions;
    std::vector<NodeSemStack> semStack;
};
