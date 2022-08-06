#pragma once

#include <vector>
#include "../lexer/lexer.h"
#include "../lexer/token.h"

struct label_t {
    std::string name;
    uint64_t address;
};

struct label_rep_t {
    std::string name;
    uint64_t machine_code_position;
    int offset;
};

class Translator {
    private:
        uint64_t position;
        std::vector<Token*> tokens;
        std::vector<uint8_t> header;
        std::vector<label_t> labels;
        std::vector<label_rep_t> label_reps; // Stores addresses where labels are used to be replaced
    public:
        Translator(std::vector<Token*> token_list, std::vector<uint8_t> header);

        std::vector<uint8_t> translate();

        bool if_nexteq(TokenType token_type);
        void expect(TokenType token_type);

    private: // registers.cpp
        void parse_mov(std::vector<uint8_t>* machine_code);
        void parse_sto(std::vector<uint8_t>* machine_code);

    private: // syscall.cpp
        void parse_cll(std::vector<uint8_t>* machine_code);
        
    private: // directives.cpp
        void translate_directive(std::vector<uint8_t>* machine_code);

    private: // flow_control.cpp
        void parse_jmp(std::vector<uint8_t>* machine_code);

    private: // stack.cpp
        void parse_psh(std::vector<uint8_t>* machine_code);
        void parse_pop(std::vector<uint8_t>* machine_code);
};