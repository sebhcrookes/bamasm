#include "globals.h"

// This namespace contains variables which can be accessed from anywhere within the assembler

namespace globals {

    std::string version = "V1.0";

    bool verbose = false;

    std::vector<std::string> mnemonics = {
        "mov",
        "sto",
        "cll",
        "jmp",
        "psh",
        "pop"
    };

    std::vector<std::string> directives = {
        "ascii",
        "asciiz",
        "array",
        "uint64"
    };
}