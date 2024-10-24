#include "lexicalanalyzer.hpp"

LexicalAnalyzer::LexicalAnalyzer(const std::string &path1, const std::string &path2): bor(path1),
    operations{"+", "-", "=", ">=", ">", "<", "<=", "++", "--", "&", "|",
               "*=", "+=", "-=", "/=", "%=", "%", "&&", "||", "/", "!", "!=", "=="} {
    GetLexemes(path2);
}

void LexicalAnalyzer::GetLexemes(const std::string &path) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    long long length = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[length];
    file.read(buffer, length);

    for (int i = 0; i < length; ++i) {
    }

    delete[] buffer;
}

bool LexicalAnalyzer::IsServiceWord(const std::string &s) {
    return bor.have_word(s);
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
    for (auto& operation : operations)
        if (s == operation)
            return 1;
    return 0;
}

bool LexicalAnalyzer::IsPunctuation(const std::string &s) {
    if (s == "}" || s == "{" || s == ";" || )
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