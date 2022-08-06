#include "translator.h"

#include <iostream>

#include "../utils.h"
#include "translator.h"

Translator::Translator(std::vector<Token*> token_list, std::vector<uint8_t> header) {
    this->position = 0;
    this->tokens = token_list;
    this->header = header;
}

std::vector<uint8_t> Translator::translate() {
    std::vector<uint8_t> machine_code;

    while(position < tokens.size()) {
        if(tokens[position]->type == TokenType::directive) {
            translate_directive(&machine_code);
        } else if(tokens[position]->type == TokenType::label) { 
            label_t label = { tokens[position]->contents, machine_code.size() };
            labels.push_back(label);
            position++;
        } else if (tokens[position]->type != TokenType::mnemonic) {
            std::cerr << "Error: Translation failed at line " << tokens[position]->line_num << ": instruction failed to parse correctly and not all tokens eaten, ended up on token '" << tokens[position]->contents << "', of type '" << type_to_string(tokens[position]->type) << "'" << std::endl;
            std::exit(1);
        } else {
            std::string mnemonic = tokens[position]->contents;
            position++;

            if(mnemonic == "mov")       parse_mov(&machine_code);
            else if(mnemonic == "cll")  parse_cll(&machine_code);
            else if(mnemonic == "jmp")  parse_jmp(&machine_code);
            else if(mnemonic == "psh")  parse_psh(&machine_code);
            else if(mnemonic == "pop")  parse_pop(&machine_code);
            else if(mnemonic == "sto")  parse_sto(&machine_code);
            else {
                std::cerr << "Error: Unknown mnemonic '" << mnemonic << "' on line " << tokens[position]->line_num << std::endl;
                std::exit(1);
            }
        }
    }

    // Perform all label replacements

    for(label_t label : labels) {
        std::cout << label.name << " : " << label.address << std::endl;
    }

    for(label_rep_t label_rep : label_reps) {
        uint64_t address = 0;
        std::cout << label_rep.name << std::endl;
        for(label_t label : labels) {
            if(label.name == label_rep.name) {
                address = label.address + label_rep.offset;
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

bool Translator::if_nexteq(TokenType token_type) {
    if(position + 1 < tokens.size()) {
        if(tokens[position]->type == token_type) {
            return true;
        }
    }
    return false;
}

void Translator::expect(TokenType token_type) {
    if(tokens[position]->type == token_type) return;

    std::cerr << "Error: Expected token of type '" << type_to_string(token_type) << "', found '" << tokens[position]->contents << "' with type '" << type_to_string(tokens[position]->type) << "' on line " << tokens[position]->line_num << std::endl;
    std::exit(1);
}