#include "borsem.hpp"

void BorSem::insert(const std::string &str, Type t) {
    node * cur = root;
    for (auto ch : str) {
        if (!cur->next.count(ch)) {
            cur->next[ch] = new node;
        }
        cur = cur->next[ch];
    }
    if (cur->is_terminal) {
        throw std::runtime_error("добавление в бор элемента, который уже есть!");
    }
    cur->is_terminal = 1;
    cur->type = t;
}

Type BorSem::get(const std::string &str) {
    node * cur = root;
    for (auto ch : str) {
        if (!cur->next.count(ch)) {
            throw std::out_of_range("получение элемента бора, который не существует");
        }
        cur = cur->next[ch];
    }
    if (!cur->is_terminal) {
        throw std::out_of_range("получение элемента бора, который не существует");
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

BorSem::BorSem(): root(new node) {};