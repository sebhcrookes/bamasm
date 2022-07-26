#include "globals.h"

namespace globals {

    std::string version = "V1.0";

    bool verbose = false;

    std::vector<std::string> mnemonics = {
        "mov",
        "cll",
        "jmp"
    };

    std::vector<std::string> directives = {
        "ascii",
        "asciiz",
        "array"
    };
}