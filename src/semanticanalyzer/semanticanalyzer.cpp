#include "semanticanalyzer.hpp"

void SemanticAnalyzer::TID::PushId(const Lexeme &lex, Type type) {
    if (identifiers.check(lex.GetContent())) {
        throw SemanticError(lex, "There is already identifier with the same name.");
    }
    identifiers.insert(lex.GetContent(), type);
}

Type SemanticAnalyzer::TID::CheckId(const Lexeme &lex) {
    if (!identifiers.check(lex.GetContent())) {
        throw SemanticError(lex, "There is no identifier with this name.");
    }
    return identifiers.get(lex.GetContent());
}

bool SemanticAnalyzer::TID::Check(const Lexeme &lex) {
    return identifiers.check(lex.GetContent());
}

SemanticAnalyzer::SemanticAnalyzer(): root(new Node), cur_scope(root), cnt_cycles(0), cur_have_return(0), cur_switch_type() {}

void SemanticAnalyzer::PushId(const Lexeme &lex, Type type) {
    cur_scope->data.PushId(lex, type);
}

Type SemanticAnalyzer::CheckId(const Lexeme &lex) {
    Node* check_scope = cur_scope;
    while (check_scope != nullptr && !check_scope->data.Check(lex)) {
        check_scope = check_scope->prev;
    }
    if (check_scope == nullptr) {
        throw SemanticError(lex, "There is no identifier with this name");
    }
    return check_scope->data.CheckId(lex);
}

void SemanticAnalyzer::CreateScope() {
    cur_scope->next.push_back(new Node);
    cur_scope->next.back()->prev = cur_scope;
    cur_scope = cur_scope->next.back();
}

void SemanticAnalyzer::ExitScope() {
    if (cur_scope->prev == nullptr)
        throw std::exception();
    cur_scope = cur_scope->prev;
}

void SemanticAnalyzer::PushFunc(std::string name, Type return_type, std::vector<Type>& formal_args, Lexeme& lex) {
    table_function.PushFunc(name, return_type, formal_args, lex);
}

bool SemanticAnalyzer::CheckFun(std::string name, std::vector<Type>& fact_args, Lexeme& lex) {
    return table_function.CheckFunc(name, fact_args, lex);
}

void SemanticAnalyzer::PushSemStack(Lexeme &lex) {
    if (lex.IsOperation()) {
        SemStack.push(Element(lex.GetContent(), lex));
    } else if (lex.IsLiteral()) {
        Type cur_type;
        if (lex.GetType() == LexemeType::StringLiteral) {
            cur_type = Type::String;
        } else if (lex.GetType() == LexemeType::Literal) {
            cur_type = Type::Int;
        } else if (lex.GetType() == LexemeType::DoubleLiteral) {
            cur_type = Type::Double;
        }
        else {
            throw SemanticError(lex);
        }
        SemStack.push(Element(cur_type, false));
    } else if (lex.IsIdentifier()) {
        Type cur_type = CheckId(lex);
        SemStack.push(Element(cur_type, true));
    } else {
        throw SemanticError(lex);
    }
}

void SemanticAnalyzer::PushSemStack(Type type) {
    SemStack.push(Element(type, 0));
}

bool SemanticAnalyzer::CheckBin() {
    Element right_operand = SemStack.top();
    SemStack.pop();
    Element operation = SemStack.top();
    SemStack.pop();
    Element left_operand = SemStack.top();
    SemStack.pop();
    if (operation.lex.AssignmentOp()) {
        if (!left_operand.is_lvalue) {
            throw SemanticError(operation.lex, "left operand of assignment expression must be lvalue.");
        }
        if (operation.content == "%=") {
            if (left_operand.type == Type::Double || right_operand.type == Type::Double) {
                throw SemanticError(operation.lex, "Types are not matching.");
            }
        }
        if (left_operand.type == Type::Int || left_operand.type == Type::Bool || left_operand.type == Type::Double) {
            if (right_operand.type != Type::Int && right_operand.type != Type::Bool &&
                right_operand.type != Type::Double) {
                throw SemanticError(operation.lex, "Types are not matching.");
            }
            SemStack.push(Element(left_operand.type, 1));
        } else if (left_operand.type == Type::String) {
            if (right_operand.type != Type::String || !(operation.content == "=" || operation.content == "+=")) {
                throw SemanticError(operation.lex, "Types are not matching.");
            } else {
                SemStack.push(Element(right_operand.type, 1));
            }
        } else if (left_operand.type == Type::Array) {
            throw SemanticError(operation.lex, "Arrays do not have an assignment operator.");
        }
    } else if (operation.lex.IsBitwise()) {
        if (left_operand.type != Type::Int && left_operand.type != Type::Bool || right_operand.type != Type::Int && right_operand.type != Type::Bool) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else {
            SemStack.push(Element(Type::Int, 0));
        }
    } else if (operation.lex.IsLogical()) {
        if (left_operand.type != Type::Int && left_operand.type != Type::Bool && left_operand.type != Type::Double || right_operand.type != Type::Int && right_operand.type != Type::Bool && right_operand.type != Type::Double) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else {
            SemStack.push((Element(Type::Bool, 0)));
        }
    } else if (operation.lex.IsEqualCompare()) {
        if (left_operand.type == Type::String && right_operand.type != Type::String || left_operand.type != Type::String && right_operand.type == Type::String) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else if (left_operand.type == Type::String && right_operand.type == Type::String) {
            SemStack.push((Element(Type::Bool, 0)));
        } else if (left_operand.type != Type::Int && left_operand.type != Type::Bool && left_operand.type != Type::Double || right_operand.type != Type::Int && right_operand.type != Type::Bool && right_operand.type != Type::Double) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else {
            SemStack.push((Element(Type::Bool, 0)));
        }
    } else if (operation.lex.IsAddMulty()) {
        if (left_operand.type == Type::String || right_operand.type == Type::String) {
            if (operation.content != "+" || left_operand.type != Type::String || right_operand.type != Type::String) {
                throw SemanticError(operation.lex, "Types are not matching.");
            }
            SemStack.push(Element(Type::String, 0));
        } else {
            if (left_operand.type != Type::Int && left_operand.type != Type::Bool &&
                left_operand.type != Type::Double || right_operand.type != Type::Int &&
                right_operand.type != Type::Bool && right_operand.type != Type::Double) {
                throw SemanticError(operation.lex, "Types are not matching.");
            }
            if (left_operand.type == Type::Double || right_operand.type == Type::Double) {
                if (operation.content == "%") {
                    throw SemanticError(operation.lex, "Types are not matching.");
                }
                SemStack.push(Element(Type::Double, 0));
            } else {
                SemStack.push(Element(Type::Int, 0));
            }
        }
    }
    return true;
}

bool SemanticAnalyzer::CheckUno() {
    Element operand = SemStack.top();
    SemStack.pop();
    Element operation = SemStack.top();
    SemStack.pop();
    if (!operand.is_lvalue) {
        throw SemanticError(operation.lex, "Operand must be lvalue");
    }
    if (operation.content == "~") {
        if (operand.type != Type::Int && operand.type != Type::Bool) {
            throw SemanticError(operation.lex, "Type isn't matching with this operation.");
        }
        SemStack.push(Element(operand.type, 1));
    }
    else if (operation.content == "-" || operation.content == "+" || operation.content == "!") {
        if (operand.type != Type::Bool && operand.type != Type::Int && operand.type != Type::Double) {
            throw SemanticError(operation.lex, "Type isn't matching with this operation.");
        }
        SemStack.push(Element(operand.type, 1));
    } else {
        if (operand.type != Type::Double && operand.type != Type::Int) {
            throw SemanticError(operation.lex, "Type isn't matching with this operation.");
        }
        SemStack.push(Element(operand.type, 1));
    }
    return true;
}

bool SemanticAnalyzer::CheckPostfix() {
    Element operation = SemStack.top();
    SemStack.pop();
    Element operand = SemStack.top();
    SemStack.pop();
    if (!operand.is_lvalue) {
        throw SemanticError(operation.lex, "Operand must be lvalue");
    }
    if (operand.type != Type::Int && operand.type != Type::Double) {
        throw SemanticError(operation.lex, "Type isn't matching with this operation.");
    }
    SemStack.push(Element(operand.type, 0));
    return true;
}

void SemanticAnalyzer::SetType(Type type) {
    cur_return_type = type;
}

Type SemanticAnalyzer::GetReturnType() {
    return cur_return_type;
}

void SemanticAnalyzer::SetReturn(bool a) {
    cur_have_return = a;
}

bool SemanticAnalyzer::GetReturn() {
    return cur_have_return;
}

Type SemanticAnalyzer::GetLastType() {
    return SemStack.top().type;
}

Type SemanticAnalyzer::FunctionsTable::GetType(std::string name) {
    return bor.GetType(name);
}

Type SemanticAnalyzer::GetFunctionType(std::string name) {
    return table_function.GetType(name);
}

void SemanticAnalyzer::PopSemStack() {
    SemStack.pop();
}

void SemanticAnalyzer::FunctionsTable::PushFunc(std::string name, Type return_type, std::vector<Type> &formal_args, Lexeme& lex) {
    if (bor.check(name)) {
        throw SemanticError(lex, "There is already a function with the same name.");
    }
    bor.insert(name, return_type, formal_args);

}

bool SemanticAnalyzer::FunctionsTable::CheckFunc(std::string name, std::vector<Type> &fact_args, Lexeme& lex) {
    if (!bor.check(name)) {
        throw SemanticError(lex, "There is no function with that name");
    }
    std::vector<Type> formal_args = bor.get_args(name);
    if (formal_args.size() != fact_args.size()) {
        throw SemanticError(lex, "The formal and factual parameters of the function are different");
    }
    for (int i = 0; i < fact_args.size(); ++i) {
        Type t1 = fact_args[i], t2 = formal_args[i];
        if ((t1 == Type::Int || t1 == Type::Double || t1 == Type::Bool) && (t2 == Type::Int || t2 == Type::Double || t2 == Type::Bool)) {
            continue;
        } else if (t1 != t2) {
            throw SemanticError(lex, "The formal and factual parameters of the function are different");
        }
    }
    return true;
}

void SemanticAnalyzer::ClearSemStack() {
    while (!SemStack.empty()) {
        SemStack.pop();
    }
}

bool SemanticAnalyzer::CheckBool(Lexeme& lex) {
    if (SemStack.empty()) {
        throw SemanticError(lex);
    }
    Element type = SemStack.top();
    SemStack.pop();
    if (!type.is_type || (type.type != Type::Int && type.type != Type::Double && type.type != Type::Bool)) {
        throw SemanticError(lex, "Expression must be bool.");
    }
    return true;
}

void SemanticAnalyzer::EnterCycle() {
    cnt_cycles++;
}

void SemanticAnalyzer::ExitSycle() {
    cnt_cycles--;
}

bool SemanticAnalyzer::CheckCycle(Lexeme& lex) {
    if (cnt_cycles <= 0) {
        throw SemanticError(lex, "There are not cycles.");
    }
    return 1;
}

void SemanticAnalyzer::InsertSwitchCase(int a, Lexeme& lex) {
    if (cur_cases.empty()) {
        cur_cases.push({});
    }
    if (cur_cases.top().find(a) != cur_cases.top().end()) {
        throw SemanticError(lex, "There is already such as literal");
    }
    cur_cases.top().insert(a);
}

void SemanticAnalyzer::ClearCases() {
    while (!cur_cases.empty()){
        cur_cases.pop();
    }
}

void SemanticAnalyzer::EnterSwitch() {
    cur_cases.push({});
}

void SemanticAnalyzer::ExitSwitch() {
    cur_cases.pop();
}

SemanticAnalyzer::SemanticError::SemanticError(const Lexeme& lex, std::string text):
    text_err("Error in line " + std::to_string(lex.GetLine()) + "; On the lexeme " + lex.GetContent() + "; " + text) {}

const char *SemanticAnalyzer::SemanticError::what() const noexcept {
    return text_err.c_str();
}