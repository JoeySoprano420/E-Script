#pragma once
#include <string>

namespace EScript {

struct Token {
    std::string type;
    std::string value;
    int line;
    int column;
    
    Token() : line(0), column(0) {}
    Token(const std::string& t, const std::string& v, int l = 0, int c = 0)
        : type(t), value(v), line(l), column(c) {}
};

} // namespace EScript
