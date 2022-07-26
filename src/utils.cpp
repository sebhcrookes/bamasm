#include "utils.h"

#include <cctype>
#include <iostream>

uint8_t get_register(std::string register_name) {
    char reg_char = tolower(register_name[0]);

    return reg_char - 'a';
}

uint64_t integer_to_hexint(std::string integer, int line_number) {
    return integer_to_hexint(std::stol(integer), line_number);
}

uint64_t integer_to_hexint(uint64_t integer, int line_number) {
    if(integer != 8 && integer != 16 && integer != 32 && integer != 64) {
        std::cerr << "Error: Failed to parse given integer size '" << integer << "' on line " << line_number << std::endl;
        std::exit(1);
    }

    switch(integer) {
        case 8: return 0x08;
        case 16: return 0x16;
        case 32: return 0x32;
        case 64: return 0x64;
    }

    return 0x08;
}

uint64_t hexint_to_integer(uint64_t hexint) {
    switch(hexint) {
        case 0x08: return 8;
        case 0x16: return 16;
        case 0x32: return 32;
        case 0x64: return 64;
    }

    return 8;
}

std::vector<uint8_t> integer_to_bytes(std::string integer, int size) {
    uint64_t int_val = std::stol(integer);

    std::vector<uint8_t> array(size / 8);
    for (int i = 0; i < size / 8; i++) {
        array[(size / 8) - 1 - i] = (int_val >> (i * 8));
    }

    return array;
}