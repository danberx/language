#include "lexeme.hpp"
#include <vector>

class Generation {
public:
    class Command : public Lexeme {
    public:
        Command(const Lexeme& lex);
        Command(int comm_num);
    private:
        bool is_command;
        int command_number;
    };
    int GetCur();
    void PushPoliz(Command command, int ind = -1);


private:
    int cur;
    std::vector<Command> poliz;
};