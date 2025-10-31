#include <cstdlib>
#include <iostream>
#include <string>

#include "ir.hpp"

namespace EScript {

// Helper function to replace file extension
std::string replaceExtension(const std::string& path, const std::string& newExt) {
    size_t lastDot = path.find_last_of('.');
    size_t lastSlash = path.find_last_of("/\\");
    
    // If there's a dot and it's after the last slash (or no slash exists)
  if (lastDot != std::string::npos && 
        (lastSlash == std::string::npos || lastDot > lastSlash)) {
        return path.substr(0, lastDot) + newExt;
    }
    
    // No extension found, just append
    return path + newExt;
}

int autoLink(const std::string& asmFile, const std::string& outFile) {
  // For Windows: create .obj and .exe
    std::string obj = replaceExtension(asmFile, ".obj");
    std::string exe = outFile + ".exe";
    
    // NASM command for Windows 64-bit
    std::string cmdAsm = "nasm -f win64 \"" + asmFile + "\" -o \"" + obj + "\"";
    
 // LD command for Windows PE format
    std::string cmdLink = "ld -m i386pep \"" + obj + "\" -o \"" + exe + "\"";
  
    std::cout << "[AutoLink] Assembling...\n";
    std::cout << "  Command: " << cmdAsm << "\n";
  
    int asmResult = system(cmdAsm.c_str());
    if (asmResult != 0) {
 std::cerr << "[AutoLink] ERROR: Assembly failed!\n";
  std::cerr << "  Make sure NASM is installed and in your PATH\n";
        return asmResult;
    }
    
    std::cout << "[AutoLink] Assembly successful: " << obj << "\n";
    std::cout << "[AutoLink] Linking...\n";
    std::cout << "  Command: " << cmdLink << "\n";
    
    int linkResult = system(cmdLink.c_str());
    if (linkResult != 0) {
        std::cerr << "[AutoLink] ERROR: Linking failed!\n";
        std::cerr << "  Make sure LD (from MinGW or LLVM) is installed and in your PATH\n";
        return linkResult;
    }
    
    std::cout << "[AutoLink] Build complete!\n";
    std::cout << "  Executable: " << exe << "\n";
    
    return 0;
}

} // namespace EScript
