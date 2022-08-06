#pragma once

#include "vector"
#include "string"

#include "token.h"

class Lexer {
    private:
        std::string filename;

        std::string current_line;
        int line_index;
    public:
        Lexer(std::string filename);
        ~Lexer();

        std::vector<Token*> lex();

        void unexpected_char_err(char character, int line_number, int index);
        bool is_mnemonic(std::string value);
        bool is_directive(std::string value);

        std::string collect_identifier(std::string line);
        std::string collect_string(std::string line);
        std::string collect_integer(std::string line, bool is_hex);

};

std::string type_to_string(TokenType type);