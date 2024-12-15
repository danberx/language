#pragma once
#include <map>
#include <vector>
#include "type.hpp"
#include "lexeme.hpp"
class BorSem {
public:
    void insert(const std::string &str, Type t);
    Type get(const std::string &str);
    bool check(const std::string &str);
    BorSem();
    std::string* get_content(std::string s);
    std::string* get_array_content(std::string s, int indx);
    void set_size(std::string s, int sz);
    void push(std::string s, std::string content);
private:
    struct node {
        std::map<char, node*> next;
        Type type;
        bool is_terminal;
        std::string content;
        std::vector<std::string> arr; // for Array-types
        node(): next({}), is_terminal(false), content("0") {}
    };
    node * root;
};