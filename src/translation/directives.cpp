#include "translator.h"

#include <iostream>
#include "../utils.h"

void Translator::translate_directive(std::vector<uint8_t>* machine_code) {
    std::string type = tokens[position]->contents;
    int line_num = tokens[position]->line_num;
    position++;

    if(type == "ascii" || type == "asciiz") {
        expect(TokenType::string_literal);
        std::string value = tokens[position]->contents;
        position++;

        for(int i = 0; i < value.length(); i++) {
            machine_code->push_back(value[i]);
        }

        if(type == "asciiz") // Only make the string null terminated if ".asciiz" is used
            machine_code->push_back(0x00);
    } else if(type == "array") {
        expect(TokenType::integer);
        uint64_t arr_size = stoi(tokens[position]->contents);
        position++;
        expect(TokenType::integer);
        uint64_t element_size = stoi(tokens[position]->contents);
        position++;

        for(int i = 0; i < arr_size; i++) {
            for(int j = 0; j < element_size; j++) {
                machine_code->push_back(0x00);
            }
        }

    } else if(type == "uint64") {
        expect(TokenType::integer);    
        uint64_t value = stoi(tokens[position]->contents);
        position++;
        
        std::vector<uint8_t> bytes = integer_to_bytes(std::to_string(value), 64);

        for(int i = 0; i < bytes.size(); i++) {
            machine_code->push_back(bytes[i]);
        }
    } else {
        std::cerr << "Error: Unknown directive '." << type << "' on line " << line_num << std::endl;
        std::exit(1);
    }
}