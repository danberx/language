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

void Poliz::Run(SemanticAnalyzer& semantic) {
    while (cur_index < poliz.size()) {
        PolizElement cur = poliz[cur_index];
        if (cur.action == Action::Element) {
            if (cur.is_lvalue) {
                Type type = semantic.CheckId(cur.lexeme);
                if (type == Type::IntArray || type == Type::DoubleArray || type == Type::BoolArray || type == Type::CharArray) {
                    Element el;
                    el.lexeme = cur.lexeme;
                    counting_stack.push(el);
                } else {
                    Element el;
                    el.lvalue_content = semantic.GetContent(cur.lexeme);
                    el.rvalue_content = *el.lvalue_content;
                    counting_stack.push(el);
                }
            }
            else {
                Element el;
                el.rvalue_content = cur.lexeme.GetContent();
                counting_stack.push(el);
            }
            cur_index++;
        }
        else if (cur.action == Action::Index) {
            Element index = counting_stack.top();
            counting_stack.pop();
            Element arr = counting_stack.top();
            counting_stack.pop();
            int indx = std::stoi(index.rvalue_content);
            Element el;
            el.lvalue_content = semantic.GetArrayContent(arr.lexeme, indx);
            el.rvalue_content = *el.lvalue_content;
            counting_stack.push(el);
            cur_index++;
        } else if (cur.action == Action::Goto) {
            cur_index = cur.goto_adress;
        } else if (cur.action == Action::FalseGoto) {
            Element el = counting_stack.top();
            counting_stack.pop();
            int c = std::stoi(el.rvalue_content);
            if (c) cur_index++;
            else cur_index = cur.goto_adress;
        } else if(cur.action == Action::Input) {
            Element el = counting_stack.top();
            counting_stack.pop();
            std::cin >> *el.lvalue_content;
        } else if (cur.action == Action::Output) {
            Element el = counting_stack.top();
            counting_stack.pop();
            std::cout << el.rvalue_content;
        } else if (cur.action == Action::Array) {
            int sz = std::stoi(counting_stack.top().rvalue_content);
            counting_stack.pop();
            semantic.SetSize(counting_stack.top().lexeme, sz);
        } else if (cur.action == Action::PushArray) {
            Element el = counting_stack.top();
            counting_stack.pop();
            Element arr = counting_stack.top();
            counting_stack.pop();
            semantic.ArrayPush(arr.lexeme, el.rvalue_content);
        } else if (cur.action == Action::Operation) {
            //... Я не хочу это делать
        } else if (cur.action == Action::FunctionCall) {
            // Это я тоже не хочу делать
        }
    }
}