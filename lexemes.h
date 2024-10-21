#include<iostream>
#include <vector>
#include <string>
#include <fstream>

struct Lexeme {
    std::string content;
    int type{};
    int line{};
    Lexeme(int t, int str, std::string& s): type(t), line(str), content(s) {}
    Lexeme() = default;
};


std::vector<Lexeme> get_lexemes(std::string& path) {
    std::vector<Lexeme> lexemes;
    std::ifstream file;
    file.open(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    long long length = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[length];
    file.read(buffer, length);
    //for (int i = 0; i < length; ++i) {
    //...
    //...
    delete[] buffer;
    return lexemes;
}