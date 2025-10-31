#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include "tokens.hpp"
#include "parser.hpp"
#include "ir.hpp"

namespace EScript {

// Forward declaration of lexer
std::vector<Token> tokenize(const std::string& src);

} // namespace EScript
