#include "lexeme.hpp"
#include "lexicalanalyzer.hpp"
#include "bor.hpp"

const std::string path_words = "../words.txt";
const std::string path_code = "../code.txt";

int main() {
    LexicalAnalyzer lexer(path_words, path_code);
    lexer.print_lexemes();
}