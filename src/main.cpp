#include "all.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"

using namespace EScript;

void printUsage() {
    std::cout << "E-Script Compiler v1.0.0\n";
    std::cout << "Usage: e-script <input.es> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o <output>    Specify output filename (default: a.out)\n";
    std::cout << "  -asm           Keep assembly file\n";
    std::cout << "  -tokens        Print tokens (debug)\n";
    std::cout << "  -ast       Print AST (debug)\n";
    std::cout << "  -ir    Print IR (debug)\n";
    std::cout << "  -h, --help     Show this help message\n";
    std::cout << "\nE-Script: Every Line Operates.\n";
}

void printTokens(const std::vector<Token>& tokens) {
    std::cout << "\n=== TOKENS ===\n";
    for (const auto& tok : tokens) {
      std::cout << "[" << tok.line << ":" << tok.column << "] " 
        << tok.type << " : '" << tok.value << "'\n";
    }
    std::cout << "=============\n\n";
}

void printAST(const Program& prog) {
    std::cout << "\n=== AST ===\n";
    std::cout << "Program {\n";
    for (const auto& op : prog.ops) {
    std::cout << "  Operation {\n";
        std::cout << "    op: " << op->op << "\n";
  std::cout << "    ident: " << op->ident << "\n";
        std::cout << "    value: " << op->value << "\n";
 if (op->nested) {
          std::cout << "    nested: { op: " << op->nested->op 
       << ", ident: " << op->nested->ident 
             << ", value: " << op->nested->value << " }\n";
        }
        std::cout << "  }\n";
    }
    std::cout << "}\n";
    std::cout << "===========\n\n";
}

void printIR(const std::vector<IRInstr>& ir) {
    std::cout << "\n=== IR ===\n";
    for (const auto& instr : ir) {
        std::cout << instr.op << " " << instr.arg1 << " " << instr.arg2;
        if (!instr.label.empty()) {
     std::cout << " [" << instr.label << "]";
      }
    std::cout << "\n";
    }
    std::cout << "==========\n\n";
}

int main(int argc, char** argv) {
    try {
      if (argc < 2) {
            printUsage();
   return 1;
        }
 
        std::string inputFile;
      std::string outputFile = "a.out";
        bool keepAsm = false;
        bool showTokens = false;
        bool showAST = false;
        bool showIR = false;
   
        // Parse command line arguments
        for (int i = 1; i < argc; ++i) {
       std::string arg = argv[i];
          
     if (arg == "-h" || arg == "--help") {
                printUsage();
        return 0;
    }
            else if (arg == "-o" && i + 1 < argc) {
     outputFile = argv[++i];
  }
         else if (arg == "-asm") {
       keepAsm = true;
       }
 else if (arg == "-tokens") {
         showTokens = true;
            }
    else if (arg == "-ast") {
     showAST = true;
            }
            else if (arg == "-ir") {
        showIR = true;
          }
        else if (arg[0] != '-') {
   inputFile = arg;
       }
            else {
        std::cerr << "Unknown option: " << arg << "\n";
          return 1;
 }
        }
        
        if (inputFile.empty()) {
  std::cerr << "Error: No input file specified\n";
         printUsage();
            return 1;
        }
        
        // Read source file
        std::cout << "[E-Script] Compiling: " << inputFile << "\n";
   std::ifstream in(inputFile);
  if (!in) {
            std::cerr << "Error: Cannot open file: " << inputFile << "\n";
          return 1;
        }
        
        std::stringstream buffer;
        buffer << in.rdbuf();
  std::string source = buffer.str();
        in.close();
        
  // Lexical analysis
        std::cout << "[Lexer] Tokenizing...\n";
     auto tokens = tokenize(source);
     std::cout << "[Lexer] Generated " << tokens.size() << " tokens\n";
        
        if (showTokens) {
          printTokens(tokens);
        }
     
     // Parsing
        std::cout << "[Parser] Building AST...\n";
 Parser parser(tokens);
        auto ast = parser.parse();
        std::cout << "[Parser] Parsed " << ast->ops.size() << " operations\n";
  
 if (showAST) {
            printAST(*ast);
        }
        
        // IR Generation
  std::cout << "[IR] Generating intermediate representation...\n";
      auto ir = generateIR(*ast);
        std::cout << "[IR] Generated " << ir.size() << " instructions\n";
        
        if (showIR) {
        printIR(ir);
        }
        
        // Code Generation
        std::string asmFile = "output.asm";
      std::cout << "[CodeGen] Generating assembly...\n";
        emitNASM(ir, asmFile);
     
        // Linking
        std::cout << "[Linker] Building executable...\n";
        int linkResult = autoLink(asmFile, outputFile);
        
        if (linkResult == 0) {
        std::cout << "\n✓ Compilation successful!\n";
     std::cout << "  Output: " << outputFile << ".exe\n";
          
    // Clean up assembly file unless -asm flag is set
     if (!keepAsm) {

            }
 }
  else {
      std::cerr << "\n✗ Compilation failed!\n";
            return 1;
        }
        
        return 0;
    }
    catch (const std::exception& e) {
     std::cerr << "\n✗ Error: " << e.what() << "\n";
        return 1;
    }
}
