#include "lexicalanalyzer.hpp"
#include <iostream>

LexicalAnalyzer::LexicalAnalyzer(const std::string &path1, const std::string &path2, const std::string &path3):
    bor_service_words(path1), bor_operations(path2), cur_lexeme(0) {
    GetLexemes(path3);
}

void LexicalAnalyzer::GetLexemes(const std::string &path) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    long long length = file.tellg(), cnt_line_feed = 1;;
    file.seekg(0, std::ios::beg);
    char* buffer = new char[length + 1];
    buffer[length] = '\n';
    file.read(buffer, length);

    for (int i = 0; i <= length;) {
        if (i < length && IsOperation(std::string(1, buffer[i]) + std::string(1, buffer[i + 1]))) {
            lexemes.emplace_back(std::string(1, buffer[i]) + std::string(1, buffer[i + 1]), LexemeType::Operation, cnt_line_feed);
            i += 2;
        }
        else if (IsOperation(std::string(1, buffer[i]))) {
            lexemes.emplace_back(std::string(1, buffer[i]), LexemeType::Operation, cnt_line_feed);
            i++;
        }
        else if (IsPunctuation(std::string(1, buffer[i]))) {
            lexemes.emplace_back(std::string(1, buffer[i]), LexemeType::Punctuation, cnt_line_feed);
            i++;
        }
        else if (IsComa(std::string(1, buffer[i]))) {
            lexemes.emplace_back(std::string(1, buffer[i]), LexemeType::Comma, cnt_line_feed);
            i++;
        }
        else if (IsPoint(std::string(1, buffer[i]))) {
            lexemes.emplace_back(std::string(1, buffer[i]), LexemeType::Point, cnt_line_feed);
            i++;
        }
        else if (IsBracket(std::string(1, buffer[i]))) {
            lexemes.emplace_back(std::string(1, buffer[i]), LexemeType::Bracket, cnt_line_feed);
            i++;
        }
        else if (isdigit(buffer[i])) {
            std::string cur;
            cur = "";
            bool ok = true;
            bool point = false;
            while (i <= length && (isdigit(buffer[i]) || buffer[i] == '.')) {
                if (buffer[i] == '.') {
                    if (point) {
                        ok = false;
                    } else {
                        point = true;
                    }
                }
                cur += buffer[i++];
            }
            if (ok) {
                lexemes.emplace_back(cur, LexemeType::Literal, cnt_line_feed);
            } else {
                lexemes.emplace_back(cur, LexemeType::Other, cnt_line_feed);
            }
        }
        else if (buffer[i] == '"') {
            std::string cur;
            cur = "";
            i++;
            bool ok = true;
            for (; i <= length; i++) {
                if (buffer[i] == '\n') {
                    ok = false;
                    break;
                }
                if (buffer[i] == '"' && !cur.empty() && cur.back() == '\\') {
                    cur.back() = '"';
                }
                else if (buffer[i] == '"') {
                    i++;
                    break;
                }
                else if (buffer[i] == 'n' && !cur.empty() && cur.back() == '\\') {
                    cur.back() = '\n';
                }
                else {
                    cur.push_back(buffer[i]);
                }
            }
            if (ok) {
                lexemes.emplace_back(cur, LexemeType::StringLiteral, cnt_line_feed);
            }
            else {
                lexemes.emplace_back(cur, LexemeType::Other, cnt_line_feed);
            }
        }
        else if (buffer[i] == ' ') {
            i++;
        }
        else if (buffer[i] == '\n') {
            cnt_line_feed++;
            i++;
        }
        else if (isdigit(buffer[i]) || isalpha(buffer[i])) {
            std::string cur = "";
            while (isdigit(buffer[i]) || isalpha(buffer[i])) {
                cur += buffer[i];
                i++;
            }
            if (IsServiceWord(cur)) {
                lexemes.emplace_back(cur, LexemeType::ServiceWord, cnt_line_feed);
            } else {
                lexemes.emplace_back(cur, LexemeType::Identifier, cnt_line_feed);
            }
        }
        else {
            lexemes.emplace_back(std::string(1, buffer[i++]), LexemeType::Other, cnt_line_feed);
        }
    }
    delete[] buffer;
}

bool LexicalAnalyzer::IsServiceWord(const std::string &s) {
    return bor_service_words.have_word(s);
}

bool LexicalAnalyzer::IsIdentifier(const std::string &s) {
    if (s.size() == 0 || isdigit(s[0]))
        return 0;
    for (const auto& ch : s)
        if (!isdigit(ch) && !isalpha(ch))
            return 0;
    return 1;
}

bool LexicalAnalyzer::IsLiteral(const std::string &s) {
    if (IsInt(s) || IsDouble(s))
        return 1;
    return 0;
}

bool LexicalAnalyzer::IsStringLiteral(const std::string &s) {
    if (s.size() < 2 || s[0] != '"' || s.back() != '"')
        return 0;
    return 1;
}

bool LexicalAnalyzer::IsOperation(const std::string &s) {
    return bor_operations.have_word(s);
}

bool LexicalAnalyzer::IsPunctuation(const std::string &s) {
    if (s == "}" || s == "{" || s == ";" || s == ":")
        return 1;
    return 0;
}

bool LexicalAnalyzer::IsComa(const std::string &s) {
    if (s == ",")
        return 1;
    return 0;
}


bool LexicalAnalyzer::IsPoint(const std::string &s) {
    if (s == ".")
        return 1;
    return 0;
}

bool LexicalAnalyzer::IsBracket(const std::string &s) {
    if (s == "(" || s == ")")
        return 1;
    return 0;
}

bool LexicalAnalyzer::IsDouble(const std::string &s) {
    try {
        stod(s);
    }
    catch (std::out_of_range) {
        return 0;
    }
    catch (std::invalid_argument) {
        return 0;
    }
    return 1;
}


bool LexicalAnalyzer::IsInt(const std::string &s) {
    try {
        stoi(s);
    }
    catch (std::out_of_range) {
        return 0;
    }
    catch (std::invalid_argument) {
        return 0;
    }
    return 1;
}

Lexeme LexicalAnalyzer::GetLex() {
    if (cur_lexeme == lexemes.size()) {
        return Lexeme("END", LexemeType::Other, -1);
    }
    return lexemes[cur_lexeme++];
}

void LexicalAnalyzer::print_lexemes() {
    Lexeme cur = GetLex();
    while (cur.GetLine() != -1) {
        std::cout << "Lexeme " << cur.GetContent() << " at line " << cur.GetLine() << " have type " << static_cast<int>(cur.GetType()) << "\n";
        cur = GetLex();
    }
    return;
}
