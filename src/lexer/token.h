#pragma once

#include <string>

enum class TokenType {
    eof,
    mnemonic,
    integer,
    identifier,
    int_literal,
    char_literal,
    register_char,
    string_literal,

    label,

    // Directives
    directive,

    reference,
    array_access
};

class Token {
    public:
        std::string filename;
        std::string contents;
        TokenType type;
        int line_num;

    public:
        Token(std::string filename, std::string contents, TokenType type, int line_num);
};