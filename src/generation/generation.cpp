#include "generation.hpp"

Generation::Command::Command(const Lexeme &lex): Lexeme(lex), is_command(0), command_number(-1) {}
Generation::Command::Command(int comm_num): is_command(1), command_number(comm_num) {}

void Generation::PushPoliz(Generation::Command command, int ind) {
    if (ind == -1)
        ind = cur++;
    poliz.push_back(command);
}