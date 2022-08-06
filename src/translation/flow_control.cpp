#include "translator.h"

void Translator::parse_jmp(std::vector<uint8_t>* machine_code) {
    machine_code->push_back(0x4C); // Opcode for jmp

    machine_code->push_back(0x00);

    label_rep_t replacement = { tokens[position]->contents, machine_code->size(), 0 };
    label_reps.push_back(replacement);

    position++;

    for(int i = 0; i < 8; i++) {
        machine_code->push_back(0x00);
    }
}