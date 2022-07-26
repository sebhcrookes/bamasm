#include "translator.h"

#include <iostream>

#include "../utils.h"

Translator::Translator(std::vector<token_t> token_list, std::vector<uint8_t> header) {
    this->position = 0;
    this->tokens = token_list;
    this->header = header;
}

std::vector<uint8_t> Translator::translate() {
    std::vector<uint8_t> machine_code;

    while(position < tokens.size()) {
        if(tokens[position].type == token_type::directive) {
            translate_directive(&machine_code);
        } else if(tokens[position].type == token_type::label) { 
            label_t label = { tokens[position].contents, machine_code.size() };
            labels.push_back(label);
            position++;
        } else if (tokens[position].type != token_type::mnemonic) {
            std::cerr << "Error: Translation failed at line " << tokens[position].line_number << ": instruction failed to parse correctly and not all tokens eaten" << std::endl;
            std::exit(1);
        } else {

            std::string mnemonic = tokens[position].contents;
            position++;

            if(mnemonic == "mov")       parse_mov(&machine_code);
            else if(mnemonic == "cll")  parse_cll(&machine_code);
            else if(mnemonic == "jmp")  parse_jmp(&machine_code, &label_reps, tokens, &position);
            else {
                std::cerr << "Error: Unknown mnemonic '" << mnemonic << "' on line " << tokens[position].line_number << std::endl;
                std::exit(1);
            }
        }
    }

    // Perform all label replacements

    for(label_rep_t label_rep : label_reps) {
        uint64_t address = 0;
        for(label_t label : labels) {
            if(label.name == label_rep.name) {
                address = label.address;
            }
        }

        std::vector<uint8_t> address_bytes = integer_to_bytes(std::to_string(address), 64);

        for(int i = 0; i < 8; i++) {
            machine_code[label_rep.machine_code_position + i] = address_bytes[i];
        }
    }

    // Add the emergency exit at the end

    machine_code.push_back(0xFF);
    machine_code.push_back(0x00);
    machine_code.push_back(0x02);
    machine_code.push_back(0x00);

    // Finally, add header to beginning of file

    machine_code.insert(machine_code.begin(), header.begin(), header.end());

    return machine_code;
}

void Translator::parse_mov(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0xAA); // Opcode for mov instruction

    uint8_t mode = 0;

    expect(token_type::register_char);
    uint8_t dest_register = get_register(tokens[position].contents);
    position++;

    if(tokens[position].type == token_type::integer) { // We are loading a value into the register as we got an integer size
        mode = 0;

        uint8_t addressing_mode = 0;
        std::vector<uint8_t> value;

        uint8_t integer_size = integer_to_hexint(tokens[position].contents, tokens[position].line_number);
        position++;

        if(tokens[position].type == token_type::int_literal) {
            addressing_mode = 0;
            value = integer_to_bytes(tokens[position].contents, hexint_to_integer(integer_size));
        }

        machine_code->push_back(mode);
        machine_code->push_back(addressing_mode);
        machine_code->push_back(integer_size);
        machine_code->push_back(dest_register);
        for(int i = 0; i < value.size(); i++) {
            machine_code->push_back(value[i]);
        }

        position++;

    } else { // We are doing a register transfer
        mode = 1;
    }
}

void Translator::parse_cll(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0xFF); // Opcode for syscall

    expect(token_type::int_literal);
    std::vector<uint8_t> syscall_type = integer_to_bytes(tokens[position].contents, 16);
    position++;

    expect(token_type::int_literal); // Deprecated second argument
    uint8_t value = stoi(tokens[position].contents);
    position++;

    machine_code->push_back(syscall_type[0]);
    machine_code->push_back(syscall_type[1]);

    machine_code->push_back(value);
}

void Translator::expect(int token_type) {
    if(tokens[position].type == token_type) return;

    std::cerr << "Error: Expected token of type '" << type_to_string(token_type) << "', found '" << tokens[position].contents << "' with type '" << type_to_string(tokens[position].type) << "' on line " << tokens[position].line_number << std::endl;
    std::exit(1);
}