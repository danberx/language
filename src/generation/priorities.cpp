#include "priorities.hpp"

Priorities::Priorities() {
    priorities[","] = 0;
    priorities["="] = 1;
    priorities["+="] = 1;
    priorities["-="] = 1;
    priorities["*="] = 1;
    priorities["/="] = 1;
    priorities["%="] = 1;
    priorities["||"] = 2;
    priorities["&&"] = 3;
    priorities["|"] = 4;
    priorities["^"] = 5;
    priorities["&"] = 6;
    priorities["=="] = 7;
    priorities["!="] = 7;
    priorities["<="] = 8;
    priorities["<"] = 8;
    priorities[">="] = 8;
    priorities[">"] = 8;
    priorities["<<"] = 9;
    priorities[">>"] = 9;
    priorities["+"] = 10;
    priorities["-"] = 10;
    priorities["*"] = 11;
    priorities["/"] = 11;
    priorities["%"] = 11;
    priorities["!"] = 12;
    priorities["~"] = 12;
    priorities["unary+"] = 12;
    priorities["unary-"] = 12;
    priorities["prefix++"] = 12;
    priorities["prefix--"] = 12;
    priorities["postfix++"] = 13;
    priorities["postfix--"] = 13;
}

int Priorities::GetPriority(const std::string operation) {
    return priorities[operation];
}