#pragma once
#include <string>
#include <vector>
#include "parser.hpp"

namespace EScript {

struct IRInstr {
  std::string op;      // operation type
 std::string arg1;// first argument
    std::string arg2;    // second argument
    std::string label;   // optional label for jumps/lanes

    IRInstr() = default;
    IRInstr(const std::string& o, const std::string& a1, const std::string& a2, const std::string& lbl = "")
    : op(o), arg1(a1), arg2(a2), label(lbl) {}
};

// Function declarations
std::vector<IRInstr> generateIR(const Program& prog);
void emitNASM(const std::vector<IRInstr>& ir, const std::string& file);
int autoLink(const std::string& asmFile, const std::string& outFile);

} // namespace EScript
