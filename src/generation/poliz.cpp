#include "poliz.hpp"

void Poliz::PushOperation(Lexeme &lex) {
    PolizElement cur;
    cur.lexeme = lex;
    cur.action = Action::Operation;
    poliz.push_back(cur);
}

void Poliz::PushOperand(Lexeme &lex, bool is_adress) {
    PolizElement cur;
    cur.lexeme = lex;
    cur.is_lvalue = is_adress;
    cur.action = Action::Element;
    poliz.push_back(cur);
}

void Poliz::Skip() {
    PolizElement cur;
    poliz.push_back(cur);
}

void Poliz::SetGoTo(int index, int adress, bool false_goto) {
    PolizElement cur;
    cur.goto_adress = adress;
    if (false_goto) {
        cur.action = Action::FalseGoto;
    } else {
        cur.action = Action::Goto;
    }
    poliz[index] = cur;
}

void Poliz::PushIndex() {
    PolizElement cur;
    cur.action = Action::Index;
    poliz.push_back(cur);
}

void Poliz::PushInput() {
    PolizElement cur;
    cur.action = Action::Input;
    poliz.push_back(cur);
}

void Poliz::PushOutput() {
    PolizElement cur;
    cur.action = Action::Output;
    poliz.push_back(cur);
}

void Poliz::PushFunctionCall(Lexeme& lex) {
    PolizElement cur;
    cur.action = Action::FunctionCall;
    cur.lexeme = lex;
    poliz.push_back(cur);
}

void Poliz::PushPushArray() {
    PolizElement cur;
    cur.action = Action::PushArray;
    poliz.push_back(cur);
}

void Poliz::PushMakeArray() {
    PolizElement cur;
    cur.action = Action::Array;
    poliz.push_back(cur);
}

int Poliz::GetCur() {
    return poliz.size();
}

void Poliz::PrintPoliz() {
    for (int i = 0; i < poliz.size(); ++i) {
        PolizElement cur = poliz[i];
        if (cur.action == Action::Operation) {
            std::cout << cur.lexeme.GetContent();
        }
        else if (cur.action == Action::Element) {
            std::cout << cur.lexeme.GetContent();
        }
        else if (cur.action == Action::Goto) {
            std::cout << cur.goto_adress << "!";
        }
        else if (cur.action == Action::FalseGoto) {
            std::cout << cur.goto_adress << "!F";
        } else if (cur.action == Action::Output) {
            std::cout << "output";
        } else if (cur.action == Action::Input) {
            std::cout << "input";
        } else if (cur.action == Action::Index) {
            std::cout << "index";
        } else if (cur.action == Action::FunctionCall) {
            std::cout << cur.lexeme.GetContent();
        } else if (cur.action == Action::PushArray) {
            std::cout << "push";
        } else if (cur.action == Action::Array) {
            std::cout << "make_array";
        }
        std::cout << "  ";
    }
}