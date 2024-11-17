#pragma once
#include <vector>
#include <map>
#include <stack>
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
    void PushSemStack(Lexeme& lex);
    bool CheckBin();
    class SemanticError : public std::exception {
    public:
        const char* what() const noexcept;
        SemanticError(const Lexeme &lex, std::string text = "");
    private:
        std::string text_err;
    };
private:
    struct Element {
        bool is_type;
        Type type; // for expressions
        std::string content; // for operations
        bool is_lvalue; // for expressions
        Lexeme lex;
        Element(Type type, bool islv): is_type(1), is_lvalue(islv), type(type) {}
        Element(const std::string& operation, Lexeme& lex): is_type(0), content(operation), lex(lex) {}
    };
    std::stack<Element> SemStack;
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
