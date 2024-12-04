#pragma once
#include <vector>
#include <map>
#include <stack>
#include <exception>
#include <set>
#include "lexeme.hpp"
#include "borsem.hpp"
#include "borfunc.hpp"

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    void PushId(const Lexeme& lex, Type type);
    Type CheckId(const Lexeme& lex);
    void CreateScope();
    void ExitScope();
    int GetScopes();
    void EnterScope(int to);
    void PushFunc(std::string name, Type return_type, std::vector<Type>& formal_args, Lexeme& lex, int adress, int index, std::vector<Lexeme>& lexes);
    bool CheckFun(std::string name, std::vector<Type>& fact_args, Lexeme& lex);
    std::vector<Lexeme>& GetArgsLexemes(Lexeme& lex);
    void PushSemStack(Lexeme& lex);
    void PushSemStack(Type type, bool lval = 0);
    bool CheckBin();
    bool CheckUno();
    bool CheckPostfix();
    void ClearSemStack();
    bool CheckBool(Lexeme& lex);
    void EnterCycle();
    void ExitSycle();
    bool CheckCycle(Lexeme& lex);
    void SetType(Type type);
    Type GetReturnType();
    void SetReturn(bool a);
    bool GetReturn();
    Type GetLastType();
    void EnterSwitch();
    void ExitSwitch();
    void InsertSwitchCase(int a, Lexeme& lex);
    void PopSemStack();
    void ClearCases();
    std::string* GetContent(Lexeme& lex);
    std::vector<std::string>* GetArray(Lexeme& lex);
    Type GetFunctionType(std::string name);
    std::string* GetArrayContent(Lexeme& lex, int index);
    int GetPolizAdress(Lexeme& lex);
    void SetSize(Lexeme& lex, int sz);
    void ArrayPush(Lexeme& lex, std::string content);
    int GetRootIndex();

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
        std::string* GetContent(Lexeme& lex);
        std::vector<std::string>* GetArray(Lexeme& lex);
        std::string* GetArrayContent(Lexeme& lex, int index);
        void SetSize(Lexeme& lex, int sz);
        void Push(Lexeme& lex, std::string content);
        TID GetCopy();
    private:
        BorSem identifiers;
    };
    class FunctionsTable {
    public:
        FunctionsTable(): bor() {}
        bool CheckFunc(std::string name, std::vector<Type>& fact_args, Lexeme& lex);
        void PushFunc(std::string name, Type return_type, std::vector<Type>& formal_args, Lexeme& lex, int adress, int index, std::vector<Lexeme>& lexes);
        Type GetType(std::string name);
        int GetTreeIndex(Lexeme& lex);
        int GetPolizAdress(Lexeme& lex);
        std::vector<Lexeme>& GetLexes(Lexeme& lex);
    private:
        FunctionBor bor;
    };
    FunctionsTable table_function;

    Type cur_return_type;
    Type cur_switch_type;
public:
    struct Node {
        std::vector<Node*> next;
        Node* prev;
        TID data;
        Node(): prev(nullptr) {}
    };
    void SetCurScope(Node* sc);
    void GoScope(int index);
    int GetIndexScope();
    Node* GetCurScope();
    Node* GetRoot();
    Node* Copy(Lexeme& lex);
    void GoRoot();
private:

    Node * root, * cur_scope;
    BorSem functions;
    bool cur_have_return;
    int cnt_cycles;
    std::stack<std::set<int>> cur_cases;
    void copy(Node*& ans, Node*& p, Node*& cur);
};
