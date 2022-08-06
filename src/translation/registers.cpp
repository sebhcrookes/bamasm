#include "translator.h"

#include "../utils.h"
#include <iostream>

void Translator::parse_mov(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0xAA); // Opcode for mov instruction

    uint8_t mode = 0;

    expect(TokenType::register_char);
    uint8_t dest_register = get_register(tokens[position]->contents);
    position++;

    if(tokens[position]->type == TokenType::integer) { // We are loading a value into the register as we got an integer size
        mode = 0;

        uint8_t addressing_mode = 0;
        std::vector<uint8_t> value;

        uint8_t integer_size = std::stoi(tokens[position]->contents);
        position++;

        if(tokens[position]->type == TokenType::identifier) {
            addressing_mode = 1;
            std::string label_name = tokens[position]->contents;
            position++;

            int index = 0;

            if(tokens[position]->type == TokenType::array_access) {
                index = stoi(tokens[position]->contents);
                position++;
            }

            uint8_t flags_byte = construct_flags_byte(0, get_integer_size(integer_size), mode, addressing_mode);

            machine_code->push_back(flags_byte);
            machine_code->push_back(dest_register);

            label_rep_t replacement = { label_name, machine_code->size(), index };
            label_reps.push_back(replacement);

            for(int i = 0; i < 8; i++) {
                machine_code->push_back(0x00);
            }

        } else if(tokens[position]->type == TokenType::int_literal) {
            addressing_mode = 0;
            value = integer_to_bytes(tokens[position]->contents, integer_size);
        
            uint8_t flags_byte = construct_flags_byte(0, get_integer_size(integer_size), mode, addressing_mode);

            machine_code->push_back(flags_byte);
            machine_code->push_back(dest_register);
            for(int i = 0; i < value.size(); i++) {
                machine_code->push_back(value[i]);
            }
            
            position++;
        }

    } else if(tokens[position]->type == TokenType::reference) {
        mode = 0;

        std::string reference = tokens[position]->contents;

        uint8_t flags_byte = construct_flags_byte(0, get_integer_size(64), mode, 0);

        machine_code->push_back(flags_byte);
        machine_code->push_back(dest_register);

        label_rep_t replacement = { tokens[position]->contents, machine_code->size(), 0 };
        label_reps.push_back(replacement);

        for(int i = 0; i < 8; i++) {
            machine_code->push_back(0xFF);
        }

        position++;

    } else { // We are doing a register transfer
        mode = 1;
    }
}

void Translator::parse_sto(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0xAB); // Opcode for mov instruction

    uint8_t mode = 0;

    expect(TokenType::register_char);
    uint8_t dest_register = get_register(tokens[position]->contents);
    position++;

    if(tokens[position]->type == TokenType::integer) { // We are loading a value into the register as we got an integer size
        mode = 0;

        uint8_t addressing_mode = 0;
        std::vector<uint8_t> value;

        uint8_t integer_size = std::stoi(tokens[position]->contents);
        position++;

        if(tokens[position]->type == TokenType::identifier) {
            addressing_mode = 1;
            std::string label_name = tokens[position]->contents;
            position++;
            
            int index = 0;

            if(tokens[position]->type == TokenType::array_access) {
                index = stoi(tokens[position]->contents);
                position++;
            }

            uint8_t flags_byte = construct_flags_byte(get_integer_size(integer_size), 0, mode, addressing_mode);

            machine_code->push_back(flags_byte);
            machine_code->push_back(dest_register);

            label_rep_t replacement = { label_name, machine_code->size(), index };
            label_reps.push_back(replacement);

            for(int i = 0; i < 8; i++) {
                machine_code->push_back(0xFF);
            }

        }
    }
}