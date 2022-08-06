#include "utils.h"

#include <cctype>
#include <iostream>

// Converts the name of a register into the integer representation
uint8_t get_register(std::string register_name) {
    char reg_char = tolower(register_name[0]);

    return reg_char - 'a';
}

uint64_t integer_to_hexint(std::string integer, int line_number) {
    return integer_to_hexint(std::stoull(integer), line_number);
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

// Converts the human-readable integer size into the value used in the flags byte
uint8_t get_integer_size(uint8_t integer_size) {
    switch(integer_size) {
        case 8: return 0;
        case 16: return 1;
        case 32: return 2;
        case 64: return 3;
    }

    return 0;
}

// Converts an integer into a byte array with big-endian byte ordering
std::vector<uint8_t> integer_to_bytes(std::string integer, int size) {
    uint64_t int_val = std::stoull(integer);

    std::vector<uint8_t> array(size / 8);
    for (int i = 0; i < size / 8; i++) {
        array[(size / 8) - 1 - i] = (int_val >> (i * 8));
    }

    return array;
}

// Takes the integer 'n', and inserts integer 'm' into it, between bytes 'i' and 'j'
// Function found at https://www.geeksforgeeks.org/inserting-m-into-n-such-that-m-starts-at-bit-j-and-ends-at-bit-i-set-2/
uint8_t insertion(uint8_t n, uint8_t m, uint8_t i, uint8_t j) {
    uint8_t clear_mask = -1 << (j + 1);
    uint8_t capture_mask = (1 << i) - 1;
 
    uint8_t captured_bits = n & capture_mask;
 
    n &= clear_mask;
    m = m << i;
    n |= m;
    n |= captured_bits;
 
    return n;
}
 

// Takes the 4, 2 byte values which should be present in the flags byte, and inserts them into an 8 bit integer
uint8_t construct_flags_byte(uint8_t addr_intsz, uint8_t intsz, uint8_t instmd, uint8_t addrmd) {
    uint8_t flags_byte = 0;
    flags_byte = insertion(flags_byte, addr_intsz, 6, 7);
    flags_byte = insertion(flags_byte, intsz,      4, 5);
    flags_byte = insertion(flags_byte, instmd,     2, 3);
    flags_byte = insertion(flags_byte, addrmd,     0, 1);

    return flags_byte;
}