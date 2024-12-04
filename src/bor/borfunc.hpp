#pragma once

#include "type.hpp"
#include "lexeme.hpp"
#include <vector>
#include <string>
#include <map>

class FunctionBor {
public:
    FunctionBor(): root(new node) {}
    bool check(const std::string& str);
    void insert(const std::string& name, Type return_type, std::vector<Type>& types, int poliz_adress, int index, std::vector<Lexeme>& lexes);
    int get_index(std::string s);
    int get_poliz_adress(std::string s);
    std::vector<Type> get_args(std::string& name);
    Type GetType(std::string);
    std::vector<Lexeme>& GetArgumentsLexemes(std::string s);
private:
    struct node {
        std::map<char, node*> next;
        bool is_terminal;
        Type return_type;
        std::vector<Type> args;
        std::vector<Lexeme> lexes;
        int poliz_adress;
        int tree_index;
        node(): is_terminal(0), next({}) {}
    };
    node* root;
};