#include "semanticanalyzer.hpp"

void SemanticAnalyzer::TID::PushId(const Lexeme &lex, Type type) {
    if (identifiers.check(lex.GetContent())) {
        throw SemanticError(lex);
    }
    identifiers.insert(lex.GetContent(), type);
}

Type SemanticAnalyzer::TID::CheckId(const Lexeme &lex) {
    if (!identifiers.check(lex.GetContent())) {
        throw SemanticError(lex);
    }
    return identifiers.get(lex.GetContent());
}

bool SemanticAnalyzer::TID::Check(const Lexeme &lex) {
    return identifiers.check(lex.GetContent());
}

SemanticAnalyzer::SemanticAnalyzer(): root(new Node), cur_scope(root) {}

void SemanticAnalyzer::PushId(const Lexeme &lex, Type type) {
    cur_scope->data.PushId(lex, type);
}

Type SemanticAnalyzer::CheckId(const Lexeme &lex) {
    Node* check_scope = cur_scope;
    while (check_scope != nullptr && !check_scope->data.Check(lex)) {
        check_scope = check_scope->prev;
    }
    if (check_scope == nullptr) {
        throw std::exception();
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

void SemanticAnalyzer::PushFunc(std::string name, std::vector<std::pair<Type, const Lexeme&>> args) {
    for (auto [arg, lex] : args) {
        name += std::to_string(static_cast<int>(arg));
    }

    if (functions.check(name)) {
        throw std::exception();
    }

    functions.insert(name, Type::Function);
    CreateScope();
    for (auto [arg, lex] : args) {
        PushId(lex, arg);
    }
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
        } else {
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
            if (right_operand.type != Type::Int || right_operand.type != Type::Bool ||
                right_operand.type != Type::Double) {
                throw SemanticError(operation.lex, "Types are not matching.");
            } else {
                SemStack.push(Element(left_operand.type, 1));
            }
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
        if (left_operand.type != Type::Int || left_operand.type != Type::Bool || right_operand.type != Type::Int || right_operand.type != Type::Bool) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else {
            SemStack.push(Element(Type::Int, 0));
        }
    } else if (operation.lex.IsLogical()) {
        if (left_operand.type != Type::Int || left_operand.type != Type::Bool || left_operand.type != Type::Double || right_operand.type != Type::Int || right_operand.type != Type::Bool || right_operand.type != Type::Double) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else {
            SemStack.push((Element(Type::Bool, 0)));
        }
    } else if (operation.lex.IsEqualCompare()) {
        if (left_operand.type == Type::String && right_operand.type != Type::String || left_operand.type != Type::String && right_operand.type == Type::String) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else if (left_operand.type == Type::String && right_operand.type == Type::String) {
            SemStack.push((Element(Type::Bool, 0)));
        } else if (left_operand.type != Type::Int || left_operand.type != Type::Bool || left_operand.type != Type::Double || right_operand.type != Type::Int || right_operand.type != Type::Bool || right_operand.type != Type::Double) {
            throw SemanticError(operation.lex, "Types are not matching.");
        } else {
            SemStack.push((Element(Type::Bool, 0)));
        }
    }
}

SemanticAnalyzer::SemanticError::SemanticError(const Lexeme &lex, std::string text):
    text_err("Error in line " + std::to_string(lex.GetLine()) + "; On the lexeme " + lex.GetContent() + "; " + text) {}