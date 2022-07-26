#include <iostream>
#include <vector>
#include <fstream>

#include "lexer/lexer.h"
#include "translation/translator.h"
#include "globals.h"
#include "args.h"

int main(int argc, char* argv[]) {

    std::vector<uint8_t> header = {
        // Bamboo Header
        'B', 'E', 'F',  // Magic values
        0,                              // Program type (userspace)
        'B', 'A', 'M', 'A', 'S', 'M', 0, 0, // Compiler name
        0, 0, 0, 0, 0, 0, 0xFF, 0xFF,   // Memory size
        0, 0, 0, 0xFF,  // Heap size
        0, 0, 0, 0xFF,  // Stack size
        0, 0, 0, 0      // Reserved bytes
    };

    parse_args(argc, argv);

    if(globals::verbose) std::cout << "[ BAMASM Assembler - Version " << globals::version << " ]" << std::endl;

    std::string assembly_file = argv[1];
    std::string output_file = argv[2];

    std::vector<token_t> tokens = lex(assembly_file); // Perform lexical analysis

    Translator* translator = new Translator(tokens, header);

    std::vector<uint8_t> machine_code = translator->translate();

    uint8_t* ptr = &machine_code[0];

    std::ofstream(output_file, std::ios::binary).write((const char*) ptr, machine_code.size());

    if(globals::verbose) std::cout << "Successfully assembled file '" << assembly_file << "' and wrote binary to file '" << output_file << "'" << std::endl;

    return 0;
}