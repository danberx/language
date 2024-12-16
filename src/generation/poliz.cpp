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

void Poliz::PushSetScope(SemanticAnalyzer::Node *sc) {
    PolizElement el;
    el.action = Action::SetScope;
    el.go = sc;
    poliz.push_back(el);
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
        } else if (cur.action == Action::SetScope) {
            std::cout << "set_scope";
        }
        std::cout << "  ";
    }
}

void Poliz::Run(SemanticAnalyzer& semantic, int index) {
    cur_index = index;
    while (cur_index < poliz.size()) {
        PolizElement cur = poliz[cur_index];
        if (cur.action == Action::Element) {
            if (cur.is_lvalue) {
                Type type = semantic.CheckId(cur.lexeme);
                if (type == Type::IntArray || type == Type::DoubleArray || type == Type::BoolArray || type == Type::CharArray) {
                    Element el;
                    el.lexeme = cur.lexeme;
                    el.type = type;
                    counting_stack.push(el);
                } else {
                    Element el;
                    el.lvalue_content = semantic.GetContent(cur.lexeme);
                    el.rvalue_content = *el.lvalue_content;
                    el.type = type;
                    counting_stack.push(el);
                }
            }
            else {
                Element el;
                if (cur.lexeme.GetType() == LexemeType::DoubleLiteral) el.type = Type::Double;
                else if (cur.lexeme.GetType() == LexemeType::StringLiteral) el.type = Type::String;
                else if (cur.lexeme.GetType() == LexemeType::Literal) el.type = Type::Int;
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
            std::getline(std::cin, *el.lvalue_content);
            cur_index++;
        } else if (cur.action == Action::Output) {
            Element el = counting_stack.top();
            counting_stack.pop();
            std::cout << el.rvalue_content;
            cur_index++;
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
            std::string operation = cur.lexeme.GetContent();
            if (operation.size() > 2) { // unary operation
                Element operand = counting_stack.top();
                counting_stack.pop();
                Element el;
                el.lvalue_content = operand.lvalue_content;
                if (operation == "!prefix") {
                    int ans = std::stoi(operand.rvalue_content);
                    ans = !ans;
                    el.type = Type::Bool;
                    el.rvalue_content = std::to_string(ans);
                    *el.lvalue_content = el.rvalue_content;
                    counting_stack.push(el);
                } else if (operation == "~prefix") {
                    int ans = std::stoi(operand.rvalue_content);
                    ans = ~ans;
                    el.rvalue_content = std::to_string(ans);
                    *el.lvalue_content = el.rvalue_content;
                    el.type = Type::Int;
                    counting_stack.push(el);
                } else if (operation == "-prefix") {
                    el.rvalue_content = "-" + el.rvalue_content;
                    *el.lvalue_content = el.rvalue_content;
                    el.type = operand.type;
                    counting_stack.push(el);
                } else if (operation == "+prefix") {
                    el.rvalue_content = operand.rvalue_content;
                    *el.lvalue_content = el.rvalue_content;
                    el.type = operand.type;
                    counting_stack.push(el);
                } else if (operation == "++prefix") {
                    if (operand.type == Type::Bool) {
                        int ans = std::stoi(operand.rvalue_content);
                        ++ans;
                        if (ans != 0) ans = 1;
                        el.rvalue_content = std::to_string(ans);
                        *el.lvalue_content = el.rvalue_content;
                    } else if (operand.type == Type::Int) {
                        int ans = std::stoi(operand.rvalue_content);
                        ++ans;
                        el.rvalue_content = std::to_string(ans);
                        *el.lvalue_content = el.rvalue_content;
                        el.type = Type::Int;
                    } else if (operand.type == Type::Double) {
                        double ans = std::stod(operand.rvalue_content);
                        ++ans;
                        el.rvalue_content = std::to_string(ans);
                        *el.lvalue_content = el.rvalue_content;
                        el.type = Type::Double;
                    }
                    counting_stack.push(el);
                } else if (operation == "--prefix") {
                    if (operand.type == Type::Bool) {
                        int ans = std::stoi(operand.rvalue_content);
                        --ans;
                        if (ans != 0) ans = 1;
                        el.rvalue_content = std::to_string(ans);
                        *el.lvalue_content = el.rvalue_content;
                    } else if (operand.type == Type::Int) {
                        int ans = std::stoi(operand.rvalue_content);
                        --ans;
                        el.rvalue_content = std::to_string(ans);
                        *el.lvalue_content = el.rvalue_content;
                        el.type = Type::Int;
                    } else if (operand.type == Type::Double) {
                        double ans = std::stod(operand.rvalue_content);
                        --ans;
                        el.rvalue_content = std::to_string(ans);
                        *el.lvalue_content = el.rvalue_content;
                        el.type = Type::Double;
                    }
                    counting_stack.push(el);
                } else if (operation == "++postfix") {
                    el.type = operand.type;
                    el.rvalue_content = operand.rvalue_content;
                    if (el.type == Type::Int) {
                        int ans = std::stoi(operand.rvalue_content);
                        ans++;
                        *operand.lvalue_content = std::to_string(ans);
                    } else if (el.type == Type::Bool) {
                        int ans = std::stoi(operand.rvalue_content);
                        ans++;
                        if (ans != 0) ans = 1;
                        *operand.lvalue_content = ans;
                    } else if (el.type == Type::Double) {
                        double ans = std::stod(operand.rvalue_content);
                        ans++;
                        *operand.lvalue_content = ans;
                    }
                    counting_stack.push(el);
                } else if (operation == "--postfix") {
                    el.type = operand.type;
                    el.rvalue_content = operand.rvalue_content;
                    if (el.type == Type::Int) {
                        int ans = std::stoi(operand.rvalue_content);
                        ans--;
                        *operand.lvalue_content = std::to_string(ans);
                    } else if (el.type == Type::Bool) {
                        int ans = std::stoi(operand.rvalue_content);
                        ans--;
                        if (ans != 0) ans = 1;
                        *operand.lvalue_content = ans;
                    } else if (el.type == Type::Double) {
                        double ans = std::stod(operand.rvalue_content);
                        ans--;
                        *operand.lvalue_content = ans;
                    }
                    counting_stack.push(el);
                }
            } else  { // binary operation
                Element operand1 = counting_stack.top();
                counting_stack.pop();
                Element operand2 = counting_stack.top();
                counting_stack.pop();
                double opr1_d = stod(operand1.rvalue_content), opr2_d = stod(operand2.rvalue_content);
                int opr1_i = stoi(operand1.rvalue_content), opr2_i = stoi(operand2.rvalue_content);
                Element res;
                if (operation == "+" || operation == "-" || operation == "/" || operation == "*" || operation == "|" || operation == "&" || operation == "%" || operation == "<<" || operation == ">>" ||
                    operation == "+=" || operation == "-=" || operation == "/=" || operation == "*=" || operation == "%=") {
                    if ((operand1.type == Type::Int || operand1.type == Type::Bool) && (operand2.type == Type::Int || operand2.type == Type::Bool)) {
                        res.type == Type::Int;
                        if (operation == "+" || operation == "+=") {
                            res.rvalue_content = std::to_string(opr1_i + opr2_i);
                        }
                        else if (operation == "-" || operation == "-=") {
                            res.rvalue_content = std::to_string(opr1_i - opr2_i);
                        }
                        else if (operation == "/" || operation == "/=") {
                            res.rvalue_content = std::to_string(opr1_i / opr2_i);
                        }
                        else if (operation == "*" || operation == "*=") {
                            res.rvalue_content = std::to_string(opr1_i * opr2_i);
                        }
                        else if (operation == "|") {
                            res.rvalue_content = std::to_string(opr1_i | opr2_i);
                        }
                        else if (operation == "&") {
                            res.rvalue_content = std::to_string(opr1_i & opr2_i);
                        }
                        else if (operation == "%" || operation == "%=") {
                            res.rvalue_content = std::to_string(opr1_i % opr2_i);
                        }
                        else if (operation == "<<") {
                            res.rvalue_content = std::to_string(opr1_i << opr2_i);
                        }
                        else if (operation == ">>") {
                            res.rvalue_content = std::to_string(opr1_i >> opr2_i);
                        }
                    }
                    else {
                        res.type == Type::Double;
                        if (operation == "+" || operation == "+=") {
                            res.rvalue_content = std::to_string(opr1_d + opr2_d);
                        }
                        else if (operation == "-" || operation == "-=") {
                            res.rvalue_content = std::to_string(opr1_d - opr2_d);
                        }
                        else if (operation == "/" || operation == "/=") {
                            res.rvalue_content = std::to_string(opr1_d / opr2_d);
                        }
                        else if (operation == "*" || operation == "*=") {
                            res.rvalue_content = std::to_string(opr1_d * opr2_d);
                        }
                        else {
                            throw std::runtime_error("я где-то ошибся");
                        }
                    }
                }
                else if (operation == ">") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d > opr2_d;
                }
                else if (operation == "<") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d < opr2_d;
                }
                else if (operation == ">=") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d >= opr2_d;
                }
                else if (operation == "<=") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d <= opr2_d;
                }
                else if (operation == "&&") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d && opr2_d;
                }
                else if (operation == "||") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d || opr2_d;
                }
                else if (operation == "==") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d == opr2_d;
                }
                else if (operation == "!=") {
                    res.type = Type::Bool;
                    res.rvalue_content = opr1_d != opr2_d;
                }
                else if (operation == "=") {

                }
            }
        } else if (cur.action == Action::FunctionCall) {

        }
        else if (cur.action == Action::SetScope) {
            semantic.SetCurScope(cur.go);
            cur_index++;
        }
    }
}