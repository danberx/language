#include "lexemes.h"
#include "bor.h"

int main() {
    std::string path = "../words.txt";
    Bor bor(path);
    for (int i = 0; i < 10; ++i) {
        std::string question;
        std::cin >> question;
        std::cout << bor.have_word(question) << "\n";
    }

}