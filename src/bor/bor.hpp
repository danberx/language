#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <map>

class Bor {
public:
    Bor(const std::string& path);
    bool have_word(const std::string& s);
    ~Bor();
protected:
    struct node {
        std::map<char, node*> next;
        bool is_terminal;
        node(): next({}), is_terminal(false) {}
    };
    node* root;
    void add_string(const std::string& s);
    void delete_node(node* v);
};