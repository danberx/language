#include "bor.hpp"

Bor::Bor(const std::string& path) {
    root = new node;
    std::ifstream file;
    file.open(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    long long length = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[length + 1];
    buffer[length] = '\n';
    file.read(buffer, length);
    std::string cur;
    cur = "";
    for (int i =0; i <= length; ++i) {
        if (buffer[i] == '\n') {
            add_string(cur);
            cur = "";
        } else if (buffer[i] != '\r'){
            cur += buffer[i];
        }
    }
    delete[] buffer;
}

bool Bor::have_word(const std::string &s) {
    node* v = root;
    for (auto c : s) {
        c -= 'a';
        if (v->next[c] == nullptr) {
            return 0;
        }
        v = v->next[c];
    }
    return v->is_terminal;
}

Bor::~Bor() {
    delete_node(root);
}

void Bor::delete_node(node* v) {
    for (int i = 0; i < v->next.size(); ++i) {
        if (v->next[i] != nullptr) {
            delete_node(v->next[i]);
        }
    }
    delete v;
}

void Bor::add_string(const std::string &s) {
    node* v = root;
    for (char c: s) {
        c -= 'a';
        if (!v->next[c]) {
            v->next[c] = new node;
        }
        v = v->next[c];
    }
    v->is_terminal = true;
}