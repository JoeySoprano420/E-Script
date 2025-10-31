#include "tokens.hpp"
#include <regex>
#include <string>
#include <vector>
#include <stdexcept>

namespace EScript {

std::vector<Token> tokenize(const std::string& src) {
  // Define token patterns in order of priority
    const std::vector<std::pair<std::string, std::string>> patterns = {
        // Comments (must be before other patterns)
{"COMMENT_MULTI", R"(\*\*[\s\S]*?\*\*)"},
        {"COMMENT_SINGLE", R"(\*[^\n]*)"},
    
        // Keywords (operation verbs)
 {"KW_MODIFY", R"(\bmodify\b)"},
     {"KW_ADJUST", R"(\badjust\b)"},
        {"KW_BYPASS", R"(\bbypass\b)"},
        {"KW_DELETE", R"(\bdelete\b)"},
        {"KW_PROCESS", R"(\bprocess\b)"},
    {"KW_CREATE", R"(\bcreate\b)"},
   {"KW_DEPLOY", R"(\bdeploy\b)"},
     {"KW_LANE", R"(\blane\b)"},
     {"KW_SYNC", R"(\bsync\b)"},
        {"KW_IF", R"(\bif\b)"},
    {"KW_ELSE", R"(\belse\b)"},
        {"KW_LOOP", R"(\bloop\b)"},
        
        // Actions (must come before identifiers)
        {"ACTION_READ", R"(\bread\b)"},
  {"ACTION_WRITE", R"(\bwrite\b)"},
    {"ACTION_PING", R"(\bping\b)"},
        {"ACTION_ANALYZE", R"(\banalyze\b)"},
        
        // Literals
        {"STRING", R"("(?:[^"\\]|\\.)*")"},
        {"NUMBER", R"(\d+\.?\d*)"},
        
        // Identifiers (after keywords)
        {"IDENT", R"([A-Za-z_][A-Za-z0-9_\-]*)"},
        
   // Operators and punctuation
        {"HASH", R"(#)"},
        {"PLUS", R"(\+)"},
        {"MINUS", R"(\-)"},
        {"LPAREN", R"(\()"},
        {"RPAREN", R"(\))"},
        {"TO", R"(\bto\b)"},
        
        // Whitespace (will be filtered out)
        {"WS", R"(\s+)"}
    };
  
    std::vector<Token> out;
    std::string rem = src;
    int line = 1;
    int column = 1;
    
    while (!rem.empty()) {
        bool matched = false;
  
        // Replace the problematic for loop with correct structured binding syntax
        for (const auto& pattern : patterns) {
            const std::string& type = pattern.first;
            const std::string& pat = pattern.second;
            std::smatch m;
            std::regex regex("^" + pat);

            if (std::regex_search(rem, m, regex)) {
                std::string match_str = m.str();

                // Skip whitespace and comments, but track line numbers
                if (type != "WS" && type != "COMMENT_SINGLE" && type != "COMMENT_MULTI") {
                    out.push_back(Token(type, match_str, line, column));
                }

                // Update position tracking
                for (char c : match_str) {
                    if (c == '\n') {
                        line++;
                        column = 1;
                    } else {
                        column++;
                    }
                }

                rem = m.suffix().str();
                matched = true;
                break;
            }
        }
        
        if (!matched) {
            throw std::runtime_error(
    "Lexer error at line " + std::to_string(line) + 
 ", column " + std::to_string(column) +
    ": Unknown token near '" + rem.substr(0, 10) + "...'"
            );
        }
    }
    
    return out;
}

} // namespace EScript
