#include "borsem.hpp"

void BorSem::insert(const std::string &str, Type t) {
    node * cur = root;
    for (auto ch : str) {
        if (!cur->next.count(ch)) {
            cur->next[ch] = new node;
        }
        cur = cur->next[ch];
    }
    if (cur->is_terminal)
        throw std::string("добавление в бор элемента, который уже есть!");

    cur->is_terminal = 1;
    cur->type = t;
}

Type BorSem::get(const std::string &str) {
    node * cur = root;
    for (auto ch : str) {
        if (!cur->next.count(ch)) {
            throw std::string("получение элемента бора, который не существует");
        }
        cur = cur->next[ch];
    }
    if (!cur->is_terminal) {
        throw std::string("получение элемента бора, который не существует");
    }
    return cur->type;
}

bool BorSem::check(const std::string &str) {
    node * cur = root;
    for (auto ch : str) {
        if (!cur->next.count(ch)) {
            return 0;
        }
        cur = cur->next[ch];
    }
    return cur->is_terminal;
}

std::string* BorSem::get_content(std::string s) {
    node* cur = root;
    for (auto c: s) {
        cur = cur->next[c];
    }
    return &cur->content;
}

std::string* BorSem::get_array_content(std::string s, int indx) {
    node* cur = root;
    for (auto c: s) {
        cur = cur->next[c];
    }
    return &cur->arr[indx];
}

void BorSem::set_size(std::string s, int sz) {
    node* cur = root;
    for (auto c: s) {
        cur = cur->next[c];
    }
    cur->arr.resize(sz);
}

void BorSem::push(std::string s, std::string content) {
    node* cur = root;
    for (auto c: s) {
        cur = cur->next[c];
    }
    cur->arr.push_back(content);
}

BorSem::BorSem(): root(new node) {};