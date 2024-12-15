#include <vector>
#include <string>
#include <iostream>
#include "lexeme.hpp"
#include "action.hpp"

class Poliz {
public:
    Poliz(): poliz() {}
    void PushOperation(Lexeme& lex);
    void PushOperand(Lexeme& lex, bool is_adress);
    void SetGoTo(int index, int adress, bool false_goto);
    void Skip();
    void PushIndex();
    void PushInput();
    void PushOutput();
    void PushFunctionCall(Lexeme& lex);
    void PushPushArray();
    void PushMakeArray();
    int GetCur();
    void PrintPoliz();
private:
    struct PolizElement {
        Action action;
        int goto_adress; // for goto (! and !F)
        bool is_lvalue; // for operands
        Lexeme lexeme;
    };
    std::vector<PolizElement> poliz;
};