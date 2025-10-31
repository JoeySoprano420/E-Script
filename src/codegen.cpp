#include "ir.hpp"
#include <fstream>
#include <iostream>

namespace EScript {

void emitNASM(const std::vector<IRInstr>& ir, const std::string& file) {
    std::ofstream out(file);
    
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + file);
    }
    
    // Emit data section
    out << "section .data\n";
    out << "    msg db 'E-Script executed successfully', 0Ah, 0\n";
    out << "    msg_len equ $ - msg\n";
 
    // Emit strings from IR
    int strCounter = 0;
    for (const auto& instr : ir) {
        if (!instr.arg2.empty() && instr.arg2[0] == '"') {
   // Extract string content (remove quotes)
     std::string content = instr.arg2.substr(1, instr.arg2.length() - 2);
  out << "    str_" << strCounter << " db '" << content << "', 0Ah, 0\n";
      out << "    str_" << strCounter << "_len equ $ - str_" << strCounter << "\n";
            strCounter++;
     }
    }
 out << "\n";
    
    // Emit BSS section for variables
    out << "section .bss\n";
    out << "    ; Reserved space for runtime variables\n\n";
    
    // Emit text section
  out << "section .text\n";
    out << "    global _start\n\n";
    out << "_start:\n";
    
    // Generate assembly for each IR instruction
    strCounter = 0;
    for (const auto& instr : ir) {
        out << "    ; " << instr.op << " " << instr.arg1 << " " << instr.arg2 << "\n";
        
  if (instr.op == "PROCESS" && instr.arg1 == "write") {
   // sys_write for Windows (using int 80h style for compatibility)
 out << "    ; Write operation: " << instr.arg2 << "\n";
    if (!instr.arg2.empty() && instr.arg2[0] == '"') {
   out << "    mov eax, 4          ; sys_write\n";
          out << "    mov ebx, 1   ; stdout\n";
        out << "    mov ecx, str_" << strCounter << "\n";
out << "    mov edx, str_" << strCounter << "_len\n";
      out << "    int 0x80\n";
     strCounter++;
            }
     }
        else if (instr.op == "LANE_START") {
            out << instr.label << ":\n";
   out << "  ; Lane " << instr.arg1 << " begins\n";
        }
        else if (instr.op == "LANE_END") {
          out << "    ; Lane " << instr.arg1 << " ends\n";
        }
   else if (instr.op == "SYNC_ALL") {
            out << "    ; Synchronization point\n";
        }
        else {
       out << "    ; Operation: " << instr.op << "\n";
    }
        out << "\n";
    }
    
    // Exit program
    out << "    ; Exit program\n";
    out << "    mov eax, 1          ; sys_exit\n";
    out << "    xor ebx, ebx        ; exit code 0\n";
    out << "int 0x80\n";
    
    out.close();
    std::cout << "[CodeGen] Generated assembly: " << file << "\n";
}

} // namespace EScript
