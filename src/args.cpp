#include "args.h"

#include "globals.h"
#include <iostream>

void parse_args(int argc, char* argv[]) {
    if(argc == 2) {
        if(std::string(argv[1]) == "--version") {
            std::cout << "bamasm " << globals::version << std::endl;
            std::exit(1);
        }
    }

    if(argc < 3) {
        std::cout << "Usage: bamasm <src_file> <output_file>" << std::endl;
        std::exit(1);
    }

    if(argc > 3) { // Additional options present
        for(int i = 3; i < argc; i++) {
            std::string option = argv[i];

            if(option == "--verbose") {
                globals::verbose = true;
            }
        }
    }
}