#include <map>
#include <string>

class Priorities {
public:
    Priorities();
    int GetPriority(const std::string operation);
private:
    std::map<std::string, int> priorities;
};