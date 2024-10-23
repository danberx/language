#pragma once

#include <fstream>
#include <vector>
#include <string>

class Bor {
public:
    Bor(const std::string& path);
    bool have_word(const std::string& s);
    ~Bor();
private:
    struct node {
        std::vector<node*> next;
        bool is_terminal;
        node(): next(std::vector<node*>(26, nullptr)), is_terminal(false) {}
    };
    node* root;
    void add_string(const std::string& s);
    void delete_node(node* v);
};