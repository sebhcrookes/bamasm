#include "translator.h"

void Translator::parse_jmp(std::vector<uint8_t>* machine_code, std::vector<label_rep_t>* label_reps, std::vector<token_t> tokens, uint64_t* position) {
    machine_code->push_back(0x4C); // Opcode for jmp

    machine_code->push_back(0x00);

    label_rep_t replacement = { tokens[*position].contents, machine_code->size() };
    label_reps->push_back(replacement);

    (*position)++;

    for(int i = 0; i < 8; i++) {
        machine_code->push_back(0x00);
    }
}