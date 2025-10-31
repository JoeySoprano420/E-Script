#include "parser.hpp"
#include <stdexcept>

namespace EScript {

Parser::Parser(std::vector<Token> t) : tokens(std::move(t)), pos(0) {}

bool Parser::eof() const {
    return pos >= tokens.size();
}

bool Parser::match(const std::string& t) const {
    return !eof() && tokens[pos].type == t;
}

Token Parser::consume() {
    if (eof()) {
        throw std::runtime_error("Unexpected end of input");
}
    return tokens[pos++];
}

Token Parser::current() const {
    if (eof()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return tokens[pos];
}

std::unique_ptr<Program> Parser::parse() {
    auto prog = std::make_unique<Program>();
  
    while (!eof()) {
        auto op = parseOperation();
        if (op) {
     prog->ops.push_back(std::move(op));
        }
  }
    
    return prog;
}

std::unique_ptr<Operation> Parser::parseOperation() {
    auto op = std::make_unique<Operation>();
    
  if (eof()) {
    return nullptr;
    }
    
    Token tok = current();
    op->line = tok.line;
    op->column = tok.column;
  
    // Parse operation keywords
    if (peek("KW_MODIFY", "KW_ADJUST", "KW_BYPASS", "KW_DELETE", 
            "KW_PROCESS", "KW_CREATE", "KW_DEPLOY", "KW_LANE", "KW_SYNC")) {
        
        op->op = consume().value;
      
        // BYPASS and SYNC might have optional identifiers
    if (op->op == "bypass" || op->op == "sync") {
      if (match("IDENT")) {
            op->ident = consume().value;
     }
        }
        // LANE requires identifier and nested operation
        else if (op->op == "lane") {
            if (match("IDENT")) {
        op->ident = consume().value;
     }
            
         // Parse nested operation for lane
    if (!match("HASH")) {
   op->nested = parseOperation();
          }
        }
        // Most operations need an identifier
        else {
      if (match("IDENT")) {
  op->ident = consume().value;
       }
 
         // Parse value/expression
       if (match("NUMBER") || match("STRING")) {
             op->value = consume().value;
            }
else if (match("IDENT")) {
   op->value = consume().value;
            }
 // Handle "to" keyword for expressions like "process write to"
            else if (match("TO")) {
     consume(); // consume "to"
       if (match("STRING") || match("IDENT")) {
              op->value = consume().value;
    }
      }
        }
  
        // Handle action keywords for process operations
        if (op->op == "process") {
      if (peek("ACTION_READ", "ACTION_WRITE", "ACTION_PING", "ACTION_ANALYZE")) {
          op->ident = consume().value;
         
    // Actions can have arguments
           if (match("STRING") || match("IDENT")) {
     op->value = consume().value;
            }
      else if (match("TO")) {
      consume(); // consume "to"
   if (match("STRING") || match("IDENT")) {
      op->value = consume().value;
      }
   }
            }
 }
        
        // Consume statement terminator
        if (match("HASH")) {
          consume();
      } else {
     throw std::runtime_error(
       "Expected '#' at end of statement at line " + 
      std::to_string(tok.line) + ", column " + std::to_string(tok.column)
            );
        }
    }
  else {
        throw std::runtime_error(
            "Unexpected token '" + tok.value + "' at line " + 
       std::to_string(tok.line) + ", column " + std::to_string(tok.column)
        );
  }
    
    return op;
}

} // namespace EScript
