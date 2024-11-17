#pragma once
#include <map>
#include "type.hpp"
#include "lexeme.hpp"
class BorSem {
public:
    void insert(const std::string &str, Type t);
    Type get(const std::string &str) const;
    bool check(const std::string &str) const;
    BorSem();
private:
    struct node {
        std::map<char, node*> next;
        Type type;
        bool is_terminal;
        node(): next({}), is_terminal(false) {}
    };
    node * root;
};