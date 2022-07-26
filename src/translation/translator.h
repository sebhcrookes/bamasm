#pragma once

#include <vector>
#include "../lexer/lexer.h"

struct label_t {
    std::string name;
    uint64_t address;
};

struct label_rep_t {
    std::string name;
    uint64_t machine_code_position;
};

class Translator {
    private:
        uint64_t position;
        std::vector<token_t> tokens;
        std::vector<uint8_t> header;
        std::vector<label_t> labels;
        std::vector<label_rep_t> label_reps; // Stores addresses where labels are used to be replaced
    public:
        Translator(std::vector<token_t> token_list, std::vector<uint8_t> header);

        std::vector<uint8_t> translate();

        void parse_mov(std::vector<uint8_t>* machine_code);
        void parse_cll(std::vector<uint8_t>* machine_code);

        void expect(int token_type);
        
        // directives.cpp

        void translate_directive(std::vector<uint8_t>* machine_code);

        // flow_control.cpp

        void parse_jmp(std::vector<uint8_t>* machine_code, std::vector<label_rep_t>* label_reps, std::vector<token_t> tokens, uint64_t* position);
};