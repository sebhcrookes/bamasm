#include "lexer.h"

#include <iostream>
#include <fstream>
#include <ctype.h>

#include "globals.h"

int line_index = 0;

std::vector<token_t> lex(std::string filename) {
    std::ifstream input_stream(filename);

    if (!input_stream) {
        std::cerr << "Error: Failed to open input file!" << std::endl;
        std::exit(1);
    }

    // Vector storing file contents
    std::vector<std::string> text;

    std::string line;

    // extract all the text from the input file
    while (getline(input_stream, line)) {

        // store each line in the vector
        text.push_back(line);
    }

    std::vector<token_t> tokens = {};

    int line_number = 1;
    for(std::string l : text) {
        while(line_index < l.length()) {
            if(isalpha(l[line_index])) { // Identifier
                std::string identifier = collect_identifier(l);

                if(identifier.length() == 1 && isalpha(identifier[0])) { // Is a register
                    token_t token = { identifier, token_type::register_char, line_number };
                    tokens.push_back(token);
                } else if(is_mnemonic(identifier)) { // Is a mnemonic
                    token_t token = { identifier, token_type::mnemonic, line_number };
                    tokens.push_back(token);
                } else if(l[line_index] == ':') { // Label
                    token_t token = { identifier, token_type::label, line_number };
                    tokens.push_back(token);
                    line_index++; // Skip the colon
                } else {
                    token_t token = { identifier, token_type::identifier, line_number };
                    tokens.push_back(token);
                }
            } else if(isdigit(l[line_index])) { // Integer
                std::string int_string = collect_integer(l, false);
                token_t token = { int_string, token_type::integer, line_number };
                tokens.push_back(token);
            } else if(l[line_index] == '#') { // Literal integer
                line_index++;

                bool is_hex = false;

                if(l[line_index] == '$') {
                    line_index++;
                    is_hex = true;
                }
                
                if(!isdigit(l[line_index])) {
                    unexpected_char_err(l[line_index], line_number, line_index + 1);
                }

                std::string int_string = collect_integer(l, is_hex);
                token_t token = { int_string, token_type::int_literal, line_number };
                tokens.push_back(token);
            } else if(l[line_index] == '$') { // Hex integer
                line_index++;
                if(!isdigit(l[line_index])) {
                    unexpected_char_err(l[line_index], line_number, line_index + 1);
                }

                std::string int_string = collect_integer(l, true);
                token_t token = { int_string, token_type::integer, line_number };
                tokens.push_back(token);
            } else if(l[line_index] == '.') { // Directive
                line_index++; // Skip the '.'
                std::string directive = collect_identifier(l); // Collect directive
                if(is_directive(directive)) {
                    token_t token = { directive, token_type::directive, line_number };
                    tokens.push_back(token);
                } else {
                    std::cerr << "Error: Unknown directive '." << directive << "' on line " << line_number << std::endl;
                    std::exit(1);
                }
            } else if(l[line_index] == '/' && l[line_index + 1] == '/') {
                break; // Skip the whole line if we encounter a comment ('//')
            } else if(l[line_index] == '"') { // String
                std::string string_val = collect_string(l);
                token_t token = { string_val, token_type::string_literal, line_number };
                tokens.push_back(token);
            } else if(l[line_index] == ' ') { // Whitespace
                line_index++;
            } else {
                std::cerr << "Error: Unknown character '" << l[line_index] << "' on line " << line_number << ", " << line_index + 1 << " characters in" << std::endl;
                std::exit(1);
            }
        }
        line_number++;
        line_index = 0;
    }

    text.clear();

    return tokens;
}

void unexpected_char_err(char character, int line_number, int index) {
    std::cerr << "Error: Unexpected character '" << character << "' on line " << line_number << ", " << index << " characters in" << std::endl;
    std::exit(1);
}

bool is_mnemonic(std::string value) {
    for(std::string mnemonic : globals::mnemonics) {
        if(mnemonic == value) {
            return true;
        }
    }
    return false;
}

bool is_directive(std::string value) {
    for(std::string directive : globals::directives) {
        if(directive == value) {
            return true;
        }
    }
    return false;
}

std::string collect_identifier(std::string line) {
    std::string collect;
    collect.push_back(line[line_index]);
    line_index++;
    while(true) {
        if(!isalnum(line[line_index])) {
            break;
        }
        collect.push_back(line[line_index]);
        line_index++;
    }

    return collect;
}

std::string collect_string(std::string line) {
    line_index++;
    
    std::string collect;
    while(true) {
        if(line[line_index] == '"') {
            break;
        }
        collect.push_back(line[line_index]);
        line_index++;
    }
    line_index++;
    return collect;
}

std::string collect_integer(std::string line, bool is_hex) {
    std::string int_string;
    int_string.push_back(line[line_index]);
    line_index++;
    while(true) {
        if(!isdigit(line[line_index]) && !is_hex)
            break;
        
        if(!isxdigit(line[line_index]) && is_hex)
            break;
        int_string.push_back(line[line_index]);
        line_index++;
    }

    if(is_hex) { // We need to convert the hex string into a decimal string
        uint64_t val = std::stoul(int_string, nullptr, 16);
        int_string = std::to_string(val);
    }

    return int_string;
}

std::string type_to_string(int type) {
    switch(type) {
        case token_type::mnemonic: {
            return "mnemonic";
        } break;

        case token_type::integer: {
            return "integer";
        } break;

        case token_type::int_literal: {
            return "int_literal";
        } break;

        case token_type::register_char: {
            return "register";
        }

        case token_type::string_literal: {
            return "string_literal";
        }

        case token_type::directive: {
            return "directive";
        }

        case token_type::label: return "label";

        default: {
            return "unknown";
        }
    }
}