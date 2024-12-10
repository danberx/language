#include "priorities.hpp"
#include <vector>
#include <string>

class Poliz {
public:
    Poliz();
private:
    struct PolizElement {
        bool is_operation;
        bool is_address; // for operands a += 3;
        bool is_lvalue; // for oparands  (true if identifier, false if literal)
        std::string content;
        PolizElement(std::string content, bool lvalue, bool adress): content(content), is_lvalue(lvalue), address(adress) {}
        PolizElement(std::string content): content(content), is_operation
    };
};