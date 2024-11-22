#include "borfunc.hpp"

void FunctionBor::insert(const std::string &name, Type return_type, std::vector<Type> &types) {
    node* cur = root;
    for (char c: name) {
        if (cur->next.find(c) == cur->next.end()) {
            cur->next[c] = new node;
        }
        cur = cur->next[c];
    }
    cur->is_terminal = 1;
    cur->return_type = return_type;
    cur->args = std::move(types);
}

bool FunctionBor::check(const std::string &str) {
    node* cur = root;
    for (char c: str) {
        if (cur == nullptr || cur->next.find(c) == cur->next.end()) {
            return 0;
        }
        cur = cur->next[c];
    }
    return cur->is_terminal;
}

std::vector<Type> FunctionBor::get_args(std::string &name) {
    node* cur = root;
    for (char c: name) {
        cur = cur->next[c];
    }
    return cur->args;
}

Type FunctionBor::GetType(std::string name) {
    node* cur = root;
    for (char c: name) {
        cur = cur->next[c];
    }
    return cur->return_type;
}