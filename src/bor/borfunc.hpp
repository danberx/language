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
    void insert(const std::string& name, Type return_type, std::vector<Type>& types);
    std::vector<Type> get_args(std::string& name);
    Type GetType(std::string);
private:
    struct node {
        std::map<char, node*> next;
        bool is_terminal;
        Type return_type;
        std::vector<Type> args;
        node(): is_terminal(0), next({}) {}
    };
    node* root;
};