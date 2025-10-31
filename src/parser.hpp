#pragma once
#include <vector>
#include <memory>
#include <string>
#include "tokens.hpp"

namespace EScript {

// Forward declarations
struct ASTNode {
    virtual ~ASTNode() = default;
    std::string type;
    int line;
    int column;
};

struct Operation : ASTNode {
    std::string op;        // modify, adjust, bypass, delete, process, etc.
    std::string ident;     // identifier or target
    std::string value;   // expression or value
    std::unique_ptr<Operation> nested;  // for nested operations (e.g., lane operations)
    
    Operation() {
    type = "Operation";
        line = 0;
        column = 0;
    }
};

struct Program : ASTNode {
    std::vector<std::unique_ptr<Operation>> ops;
    
  Program() {
    type = "Program";
        line = 0;
     column = 0;
    }
};

class Parser {
    std::vector<Token> tokens;
    size_t pos;
    
    bool eof() const;
    bool match(const std::string& t) const;
    
    template<typename... T>
    bool peek(T... names) const {
    std::string arr[] = {names...};
  for (const auto& n : arr) {
  if (match(n)) return true;
    }
    return false;
    }
    
    Token consume();
Token current() const;
    
public:
    explicit Parser(std::vector<Token> t);
  
    std::unique_ptr<Program> parse();
    std::unique_ptr<Operation> parseOperation();
};

} // namespace EScript
