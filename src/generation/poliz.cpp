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

void Poliz::PushReturn() {
    PolizElement el;
    el.action = Action::Return;
    poliz.push_back(el);
}

void Poliz::PushExitScope() {
    PolizElement el;
    el.action = Action::ExitScope;
    poliz.push_back(el);
}

void Poliz::PushGoScope(int index) {
    PolizElement el;
    el.action = Action::GoScope;
    el.goto_adress = index;
    poliz.push_back(el);
}

int Poliz::GetCur() {
    return poliz.size();
}

void Poliz::PrintPoliz() {
    for (int i = 0; i < poliz.size(); ++i) {
        PolizElement cur = poliz[i];
        std::cout << i << ":";
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
        } else if (cur.action == Action::Return) {
            std::cout << "return";
        } else if (cur.action == Action::ExitScope) {
            std::cout << "exit_scope";
        } else if (cur.action == Action::GoScope) {
            std::cout << "go_scope" << cur.goto_adress;
        }
        std::cout << " \n";
    }
}

void Poliz::Run(SemanticAnalyzer& semantic, int index, SemanticAnalyzer::Node* root) {
    semantic.GoRoot();
    cur_index = 0;
    int cnt = 0;
    std::stack<Element>* counting_stack;
    std::stack<Element> st;
    counting_stack = &st;
    for (cur_index; cur_index < index;) {
        PolizElement cur = poliz[cur_index];
        if (cur.action == Action::Element && !cnt) {
            if (cur.is_lvalue) {
                Type type = semantic.CheckId(cur.lexeme);
                if (type == Type::IntArray || type == Type::DoubleArray || type == Type::BoolArray || type == Type::CharArray) {
                    Element el;
                    el.lexeme = cur.lexeme;
                    el.type = type;
                    counting_stack->push(el);
                } else {
                    Element el;
                    el.lvalue_content = semantic.GetContent(cur.lexeme);
                    el.rvalue_content = *el.lvalue_content;
                    el.type = type;
                    el.lexeme =cur.lexeme;
                    counting_stack->push(el);
                }
            }
            else {
                Element el;
                if (cur.lexeme.GetType() == LexemeType::DoubleLiteral) el.type = Type::Double;
                else if (cur.lexeme.GetType() == LexemeType::StringLiteral) el.type = Type::String;
                else if (cur.lexeme.GetType() == LexemeType::Literal) el.type = Type::Int;
                el.rvalue_content = cur.lexeme.GetContent();

                counting_stack->push(el);
            }
            cur_index++;
        }
        else if (cur.action == Action::Index && !cnt) {
            Element index = counting_stack->top();
            counting_stack->pop();
            Element arr = counting_stack->top();
            counting_stack->pop();
            int indx = std::stoi(index.rvalue_content);
            Element el;
            if (arr.type == Type::String) {
                el.rvalue_content = arr.rvalue_content[indx];
                el.type = Type::Char;
            }
            else {
                el.lvalue_content = semantic.GetArrayContent(arr.lexeme, indx);
                el.rvalue_content = *el.lvalue_content;
                if (arr.type == Type::IntArray) el.type = Type::Int;
                else if (arr.type == Type::DoubleArray) el.type = Type::Double;
                else if (arr.type == Type::BoolArray) el.type = Type::Bool;
            }
            counting_stack->push(el);
            cur_index++;
        } else if (cur.action == Action::Array && !cnt) {
            int sz = std::stoi(counting_stack->top().rvalue_content);
            counting_stack->pop();
            semantic.SetSize(counting_stack->top().lexeme, sz);
            cur_index++;
        } else if (cur.action == Action::GoScope) {
            cnt++;
            cur_index++;
        } else if (cur.action == Action::ExitScope) {
            cnt--;
            cur_index++;
        }
        else if (cur.action == Action::Operation && !cnt) {
            std::string operation = cur.lexeme.GetContent();
            if (operation.size() > 2) { // unary operation
                Element operand = counting_stack->top();
                counting_stack->pop();
                Element el;
                el.lvalue_content = operand.lvalue_content;
                if (operation == "!prefix") {
                    int ans = std::stoi(operand.rvalue_content);
                    ans = !ans;
                    el.type = Type::Bool;
                    el.rvalue_content = std::to_string(ans);
                    counting_stack->push(el);
                } else if (operation == "~prefix") {
                    int ans = std::stoi(operand.rvalue_content);
                    ans = ~ans;
                    el.rvalue_content = std::to_string(ans);
                    el.type = Type::Int;
                    counting_stack->push(el);
                } else if (operation == "-prefix") {
                    if (operand.rvalue_content[0] == '-') {
                        el.rvalue_content = operand.rvalue_content.substr(1, operand.rvalue_content.size() - 1);
                    }
                    else el.rvalue_content = "-" + operand.rvalue_content;
                    el.type = operand.type;
                    counting_stack->push(el);
                } else if (operation == "+prefix") {
                    el.rvalue_content = operand.rvalue_content;
                    el.type = operand.type;
                    counting_stack->push(el);
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
                    counting_stack->push(el);
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
                    counting_stack->push(el);
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
                    counting_stack->push(el);
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
                    counting_stack->push(el);
                }
            } else  { // binary operation
                Element operand2 = counting_stack->top();
                counting_stack->pop();
                Element operand1 = counting_stack->top();
                counting_stack->pop();
                if (operand1.type == Type::String || operand2.type == Type::String) {
                    Element el;
                    if (operation == "+") {
                        el.type = Type::String;
                        el.rvalue_content = operand1.rvalue_content + operand2.rvalue_content;
                    } else if (operation == "+=") {
                        el.type = Type::String;
                        el.rvalue_content = operand1.rvalue_content + operand2.rvalue_content;
                        *el.lvalue_content = el.rvalue_content;
                        *operand1.lvalue_content = el.rvalue_content;
                    } else if (operation == "=") {
                        el.type = Type::String;
                        el.rvalue_content = operand2.rvalue_content;
                        *el.lvalue_content = el.rvalue_content;
                        *operand1.lvalue_content = el.rvalue_content;
                    } else if (operation == "==") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content == operand2.rvalue_content);
                    }  else if (operation == "<=") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content <= operand2.rvalue_content);
                    }
                    else if (operation == ">=") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content >= operand2.rvalue_content);
                    } else if (operation == "<") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content < operand2.rvalue_content);
                    }
                    else if (operation == ">") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content <= operand2.rvalue_content);
                    } else if (operation == "!=") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content != operand2.rvalue_content);
                    }
                    counting_stack->push(el);
                    cur_index++;
                    continue;
                }
                double opr1_d = stod(operand1.rvalue_content), opr2_d = stod(operand2.rvalue_content);
                int opr1_i = stoi(operand1.rvalue_content), opr2_i = stoi(operand2.rvalue_content);
                Element res;
                if (operation == "+" || operation == "-" || operation == "/" || operation == "*" || operation == "|" || operation == "&" || operation == "%" || operation == "<<" || operation == ">>" ||
                    operation == "+=" || operation == "-=" || operation == "/=" || operation == "*=" || operation == "%=") {
                    if ((operand1.type == Type::Int || operand1.type == Type::Bool) && (operand2.type == Type::Int || operand2.type == Type::Bool)) {

                        res.type = Type::Int;

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
                        res.type = Type::Double;
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
                    res.rvalue_content = std::to_string(opr1_d > opr2_d);
                }
                else if (operation == "<") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d < opr2_d);
                }
                else if (operation == ">=") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d >= opr2_d);
                }
                else if (operation == "<=") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d <= opr2_d);
                }
                else if (operation == "&&") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d && opr2_d);
                }
                else if (operation == "||") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d || opr2_d);
                }
                else if (operation == "==") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d == opr2_d);
                }
                else if (operation == "!=") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d != opr2_d);
                }
                else if (operation == "=") {
                    res.type = operand1.type;
                    res.rvalue_content = operand2.rvalue_content;
                }
                if (operation == "+=" || operation == "-=" || operation == "/=" || operation == "*=" || operation == "%=" || operation == "=") {
                    res.lvalue_content = operand1.lvalue_content;
                    *res.lvalue_content = res.rvalue_content;
                    res.lexeme = operand1.lexeme;
                }
                counting_stack->push(res);
            }
            cur_index++;
        } else cur_index++;
    }

    semantic.GoRoot();
    cur_index = index;
    CallElement callel;
    callel.cur_counting_stack = std::stack<Element>();
    callel.index = index;
    callel.from = root;
    callel.function_type = Type::Int;
    call_stack.push(callel);
    counting_stack = &call_stack.top().cur_counting_stack;
    while (cur_index < poliz.size()) {
        PolizElement cur = poliz[cur_index];
        if (cur.action == Action::Element) {
            if (cur.is_lvalue) {
                Type type = semantic.CheckId(cur.lexeme);
                if (type == Type::IntArray || type == Type::DoubleArray || type == Type::BoolArray || type == Type::CharArray) {
                    Element el;
                    el.lexeme = cur.lexeme;
                    el.type = type;
                    counting_stack->push(el);
                } else {
                    Element el;
                    el.lvalue_content = semantic.GetContent(cur.lexeme);
                    el.rvalue_content = *el.lvalue_content;
                    el.type = type;
                    el.lexeme =cur.lexeme;
                    counting_stack->push(el);
                }
            }
            else {
                Element el;
                if (cur.lexeme.GetType() == LexemeType::DoubleLiteral) el.type = Type::Double;
                else if (cur.lexeme.GetType() == LexemeType::StringLiteral) el.type = Type::String;
                else if (cur.lexeme.GetType() == LexemeType::Literal) el.type = Type::Int;
                el.rvalue_content = cur.lexeme.GetContent();

                counting_stack->push(el);
            }
            cur_index++;
        }
        else if (cur.action == Action::Index) {
            Element index = counting_stack->top();
            counting_stack->pop();
            Element arr = counting_stack->top();
            counting_stack->pop();
            int indx = std::stoi(index.rvalue_content);
            Element el;
            if (arr.type == Type::String) {
                el.rvalue_content = arr.rvalue_content[indx];
                el.type = Type::Char;
            }
            else {
                el.lvalue_content = semantic.GetArrayContent(arr.lexeme, indx);
                el.rvalue_content = *el.lvalue_content;
                if (arr.type == Type::IntArray) el.type = Type::Int;
                else if (arr.type == Type::DoubleArray) el.type = Type::Double;
                else if (arr.type == Type::BoolArray) el.type = Type::Bool;
            }
            counting_stack->push(el);
            cur_index++;
        } else if (cur.action == Action::Goto) {
            cur_index = cur.goto_adress;
        } else if (cur.action == Action::FalseGoto) {
            Element el = counting_stack->top();
            counting_stack->pop();
            int c = std::stoi(el.rvalue_content);
            if (c) cur_index++;
            else cur_index = cur.goto_adress;
        } else if(cur.action == Action::Input) {
            Element el = counting_stack->top();
            counting_stack->pop();
            std::getline(std::cin, *el.lvalue_content);
            el.rvalue_content = *el.lvalue_content;
            cur_index++;
        } else if (cur.action == Action::Output) {
            Element el = counting_stack->top();
            counting_stack->pop();
            std::cout << el.rvalue_content;
            cur_index++;
        } else if (cur.action == Action::Array) {
            int sz = std::stoi(counting_stack->top().rvalue_content);
            counting_stack->pop();
            semantic.SetSize(counting_stack->top().lexeme, sz);
            cur_index++;
        } else if (cur.action == Action::PushArray) {
            Element el = counting_stack->top();
            counting_stack->pop();
            Element arr = counting_stack->top();
            counting_stack->pop();
            semantic.ArrayPush(arr.lexeme, el.rvalue_content);
            cur_index++;
        } else if (cur.action == Action::Operation) {
            std::string operation = cur.lexeme.GetContent();
            if (operation.size() > 2) { // unary operation
                Element operand = counting_stack->top();
                counting_stack->pop();
                Element el;
                el.lvalue_content = operand.lvalue_content;
                if (operation == "!prefix") {
                    int ans = std::stoi(operand.rvalue_content);
                    ans = !ans;
                    el.type = Type::Bool;
                    el.rvalue_content = std::to_string(ans);
                    counting_stack->push(el);
                } else if (operation == "~prefix") {
                    int ans = std::stoi(operand.rvalue_content);
                    ans = ~ans;
                    el.rvalue_content = std::to_string(ans);
                    el.type = Type::Int;
                    counting_stack->push(el);
                } else if (operation == "-prefix") {
                    if (operand.rvalue_content[0] == '-') {
                        el.rvalue_content = operand.rvalue_content.substr(1, operand.rvalue_content.size() - 1);
                    }
                    else el.rvalue_content = "-" + operand.rvalue_content;
                    el.type = operand.type;
                    counting_stack->push(el);
                } else if (operation == "+prefix") {
                    el.rvalue_content = operand.rvalue_content;
                    el.type = operand.type;
                    counting_stack->push(el);
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
                    counting_stack->push(el);
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
                    counting_stack->push(el);
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
                    counting_stack->push(el);
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
                    counting_stack->push(el);
                }
            } else  { // binary operation
                Element operand2 = counting_stack->top();
                counting_stack->pop();
                Element operand1 = counting_stack->top();
                counting_stack->pop();
                if (operand1.type == Type::String || operand2.type == Type::String) {
                    Element el;
                    if (operation == "+") {
                        el.type = Type::String;
                        el.rvalue_content = operand1.rvalue_content + operand2.rvalue_content;
                    } else if (operation == "+=") {
                        el.type = Type::String;
                        el.rvalue_content = operand1.rvalue_content + operand2.rvalue_content;
                        *el.lvalue_content = el.rvalue_content;
                        *operand1.lvalue_content = el.rvalue_content;
                    } else if (operation == "=") {
                        el.type = Type::String;
                        el.rvalue_content = operand2.rvalue_content;
                        *el.lvalue_content = el.rvalue_content;
                        *operand1.lvalue_content = el.rvalue_content;
                    } else if (operation == "==") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content == operand2.rvalue_content);
                    }  else if (operation == "<=") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content <= operand2.rvalue_content);
                    }
                    else if (operation == ">=") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content >= operand2.rvalue_content);
                    } else if (operation == "<") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content < operand2.rvalue_content);
                    }
                    else if (operation == ">") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content <= operand2.rvalue_content);
                    } else if (operation == "!=") {
                        el.type = Type::Bool;
                        el.rvalue_content = std::to_string(operand1.rvalue_content != operand2.rvalue_content);
                    }
                    counting_stack->push(el);
                    cur_index++;
                    continue;
                }
                double opr1_d = stod(operand1.rvalue_content), opr2_d = stod(operand2.rvalue_content);
                int opr1_i = stoi(operand1.rvalue_content), opr2_i = stoi(operand2.rvalue_content);
                Element res;
                if (operation == "+" || operation == "-" || operation == "/" || operation == "*" || operation == "|" || operation == "&" || operation == "%" || operation == "<<" || operation == ">>" ||
                    operation == "+=" || operation == "-=" || operation == "/=" || operation == "*=" || operation == "%=") {
                    if ((operand1.type == Type::Int || operand1.type == Type::Bool) && (operand2.type == Type::Int || operand2.type == Type::Bool)) {

                        res.type = Type::Int;

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
                        res.type = Type::Double;
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
                    res.rvalue_content = std::to_string(opr1_d > opr2_d);
                }
                else if (operation == "<") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d < opr2_d);
                }
                else if (operation == ">=") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d >= opr2_d);
                }
                else if (operation == "<=") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d <= opr2_d);
                }
                else if (operation == "&&") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d && opr2_d);
                }
                else if (operation == "||") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d || opr2_d);
                }
                else if (operation == "==") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d == opr2_d);
                }
                else if (operation == "!=") {
                    res.type = Type::Bool;
                    res.rvalue_content = std::to_string(opr1_d != opr2_d);
                }
                else if (operation == "=") {
                    res.type = operand1.type;
                    res.rvalue_content = operand2.rvalue_content;
                }
                if (operation == "+=" || operation == "-=" || operation == "/=" || operation == "*=" || operation == "%=" || operation == "=") {
                    res.lvalue_content = operand1.lvalue_content;
                    *res.lvalue_content = res.rvalue_content;
                    res.lexeme = operand1.lexeme;
                }
                counting_stack->push(res);
            }
            cur_index++;
        } else if (cur.action == Action::FunctionCall) {
            CallElement elem;
            elem.function_type = semantic.GetFunctionType(cur.lexeme.GetContent());
            elem.index = cur_index + 1;
            elem.from = semantic.GetCurScope();
            SemanticAnalyzer::Node* go = semantic.Copy(cur.lexeme);
            semantic.SetCurScope(go);
            std::vector<Lexeme>& args = semantic.GetArgsLexemes(cur.lexeme);
            for (int i = args.size() - 1; i >= 0; --i) {
                Element el = counting_stack->top();
                counting_stack->pop();
                if (el.type == Type::IntArray || el.type == Type::DoubleArray || el.type == Type::BoolArray || el.type == Type::CharArray) {
                    std::vector<std::string>* cont = semantic.GetArray(args[i]);
                    *cont = *semantic.GetArray(el.lexeme);
                } else {
                    std::string* content = semantic.GetContent(args[i]);
                    *content = el.rvalue_content;
                }
            }
            cur_index = semantic.GetPolizAdress(cur.lexeme);
            call_stack.push(elem);
            counting_stack = &call_stack.top().cur_counting_stack;
        }
        else if (cur.action == Action::Return) {
            bool not_void = (call_stack.top().function_type != Type::Void);
            Element res;
            if (not_void) {
                res = call_stack.top().cur_counting_stack.top();
            }
            semantic.SetCurScope(call_stack.top().from);
            cur_index = call_stack.top().index;
            call_stack.pop();
            if (call_stack.empty()) return;
            if (not_void) {
                call_stack.top().cur_counting_stack.push(res);
            }
            counting_stack = &call_stack.top().cur_counting_stack;
        }
        else if (cur.action == Action::SetScope) {
            semantic.SetCurScope(cur.go);
            cur_index++;
        } else if (cur.action == Action::ExitScope) {
            semantic.ExitScope();
            cur_index++;
        } else if (cur.action == Action::GoScope) {
            semantic.GoScope(cur.goto_adress);
            cur_index++;
        }
    }
}