#pragma once

#include "vector"
#include "string"

enum token_type {
    eof,
    mnemonic,
    size,
    integer,
    identifier,
    int_literal,
    char_literal,
    register_char,
    string_literal,

    label,

    // Directives
    directive
};

struct token_t {
    std::string contents;
    int type;
    int line_number;
};

void unexpected_char_err(char character, int line_number, int index);

std::vector<token_t> lex(std::string filename);
bool is_mnemonic(std::string value);
bool is_directive(std::string value);
std::string collect_identifier(std::string line);
std::string collect_string(std::string line);
std::string collect_integer(std::string line, bool is_hex);

std::string type_to_string(int type);