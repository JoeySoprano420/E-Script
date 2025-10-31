#include "ir.hpp"
#include <algorithm>

namespace EScript {

std::vector<IRInstr> generateIR(const Program& prog) {
    std::vector<IRInstr> ir;
 int labelCounter = 0;

    for (const auto& opPtr : prog.ops) {
   const auto& op = *opPtr;
   
        // Generate IR based on operation type
 if (op.op == "modify") {
            ir.push_back(IRInstr("MODIFY", op.ident, op.value));
        }
        else if (op.op == "adjust") {
    ir.push_back(IRInstr("ADJUST", op.ident, op.value));
        }
      else if (op.op == "bypass") {
  ir.push_back(IRInstr("BYPASS", op.ident, ""));
     }
        else if (op.op == "delete") {
      ir.push_back(IRInstr("DELETE", op.ident, ""));
        }
  else if (op.op == "process") {
       ir.push_back(IRInstr("PROCESS", op.ident, op.value));
        }
        else if (op.op == "create") {
 ir.push_back(IRInstr("CREATE", op.ident, op.value));
        }
        else if (op.op == "deploy") {
          ir.push_back(IRInstr("DEPLOY", op.ident, op.value));
        }
    else if (op.op == "lane") {
         // Generate lane label
      std::string laneLabel = "lane_" + op.ident;
  ir.push_back(IRInstr("LANE_START", op.ident, "", laneLabel));
          
      // Process nested operation
   if (op.nested) {
 // Recursively generate IR for nested operation
     if (op.nested->op == "process") {
  ir.push_back(IRInstr("PROCESS", op.nested->ident, op.nested->value, laneLabel));
      } else {
 ir.push_back(IRInstr(op.nested->op, op.nested->ident, op.nested->value, laneLabel));
    }
         }
    
  ir.push_back(IRInstr("LANE_END", op.ident, "", laneLabel));
        }
        else if (op.op == "sync") {
      if (!op.ident.empty()) {
             ir.push_back(IRInstr("SYNC", op.ident, ""));
    } else {
  ir.push_back(IRInstr("SYNC_ALL", "", ""));
            }
    }
}
    
    return ir;
}

} // namespace EScript
