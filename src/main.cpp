#include "lexemeanalyzer/lexicalanalyzer.hpp"
#include "syntacticanalyzer/syntacticanalyzer.hpp"
#include <iostream>

const std::string path_words = "../src/txt/words.txt";
const std::string path_operations = "../src/txt/operations.txt";
const std::string path_code = "../code.txt";

int main() {
    SyntacticAnalyzer a(path_words, path_operations, path_code);
cd}