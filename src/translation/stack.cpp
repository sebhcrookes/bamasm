#include "translator.h"

#include "../utils.h"

void Translator::parse_psh(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0x10);

    expect(TokenType::register_char);
    uint8_t src_register = get_register(tokens[position]->contents);
    position++;

    expect(TokenType::integer);
    uint8_t integer_size = integer_to_hexint(tokens[position]->contents, tokens[position]->line_num);
    position++;

    machine_code->push_back(src_register);
    machine_code->push_back(integer_size);
}

void Translator::parse_pop(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0x11);

    expect(TokenType::register_char);
    uint8_t dest_register = get_register(tokens[position]->contents);
    position++;

    expect(TokenType::integer);
    uint8_t integer_size = integer_to_hexint(tokens[position]->contents, tokens[position]->line_num);
    position++;

    machine_code->push_back(dest_register);
    machine_code->push_back(integer_size);
}