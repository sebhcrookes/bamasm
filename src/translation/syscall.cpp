#include "translator.h"

#include "../utils.h"

void Translator::parse_cll(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0xFF); // Opcode for syscall

    expect(TokenType::int_literal);
    std::vector<uint8_t> syscall_type = integer_to_bytes(tokens[position]->contents, 16);
    position++;

    expect(TokenType::int_literal); // Deprecated second argument
    uint8_t value = stoi(tokens[position]->contents);
    position++;

    machine_code->push_back(syscall_type[0]);
    machine_code->push_back(syscall_type[1]);

    machine_code->push_back(value);
}
