#include "lexer.h"

#include <iostream>
#include <fstream>
#include <ctype.h>

#include "globals.h"

Lexer::Lexer(std::string filename) {
    this->filename = filename;
}


Lexer::~Lexer() {

}

std::vector<Token*> Lexer::lex() {
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

    std::vector<Token*> tokens = {};

    int line_number = 1;
    for(std::string l : text) {
        line_index = 0;
        while(line_index < l.length()) {
            if(isalpha(l[line_index])) { // Identifier
                std::string identifier = collect_identifier(l);
                if(l[line_index] == ':') { // Label
                    Token* token = new Token(filename, identifier, TokenType::label, line_number);
                    tokens.push_back(token);
                    line_index++; // Skip the colon
                } else if(identifier.length() == 1 && isalpha(identifier[0])) { // Is a register
                    Token* token = new Token(filename, identifier, TokenType::register_char, line_number);
                    tokens.push_back(token);
                } else if(is_mnemonic(identifier)) { // Is a mnemonic
                    Token* token = new Token(filename, identifier, TokenType::mnemonic, line_number);
                    tokens.push_back(token);
                } else {
                    Token* token = new Token(filename, identifier, TokenType::identifier, line_number);
                    tokens.push_back(token);
                }
            } else if(isdigit(l[line_index])) { // Integer
                std::string int_string = collect_integer(l, false);
                Token* token = new Token(filename, int_string, TokenType::integer, line_number);
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
                Token* token = new Token(filename, int_string, TokenType::int_literal, line_number);
                tokens.push_back(token);
            } else if(l[line_index] == '$') { // Hex integer
                line_index++;
                if(!isdigit(l[line_index])) {
                    unexpected_char_err(l[line_index], line_number, line_index + 1);
                }

                std::string int_string = collect_integer(l, true);
                Token* token = new Token(filename, int_string, TokenType::integer, line_number);
                tokens.push_back(token);
            } else if(l[line_index] == '&') { // Reference
                line_index++;
                std::string value = collect_identifier(l);
                Token* token = new Token(filename, value, TokenType::reference, line_number);
                tokens.push_back(token);
            } else if(l[line_index] == '[') { // Array access
                line_index++;
                std::string int_string = collect_integer(l, false);
                if(l[line_index] != ']') {
                    std::cerr << "Error: Unnclosed square brackets on line " << line_number << std::endl;
                    std::exit(1);
                }
                line_index++;
                Token* token = new Token(filename, int_string, TokenType::array_access, line_number);
                tokens.push_back(token);
            } else if(l[line_index] == '.') { // Directive
                line_index++; // Skip the '.'
                std::string directive = collect_identifier(l); // Collect directive
                if(is_directive(directive)) {
                    Token* token = new Token(filename, directive, TokenType::directive, line_number);
                    tokens.push_back(token);
                } else {
                    std::cerr << "Error: Unknown directive '." << directive << "' on line " << line_number << std::endl;
                    std::exit(1);
                }
            } else if(l[line_index] == '/' && l[line_index + 1] == '/') {
                break; // Skip the whole line if we encounter a comment ('//')
            } else if(l[line_index] == '"') { // String
                std::string string_val = collect_string(l);
                Token* token = new Token(filename, string_val, TokenType::string_literal, line_number);
                tokens.push_back(token);
            } else if(l[line_index] == ' ') { // Whitespace
                line_index++;
            } else if(l[line_index] == 13) {
                line_index++; // Skip this character
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

void Lexer::unexpected_char_err(char character, int line_number, int index) {
    std::cerr << "Error: Unexpected character '" << character << "' on line " << line_number << ", " << index << " characters in" << std::endl;
    std::exit(1);
}

bool Lexer::is_mnemonic(std::string value) {
    for(std::string mnemonic : globals::mnemonics) {
        if(mnemonic == value) {
            return true;
        }
    }
    return false;
}

bool Lexer::is_directive(std::string value) {
    for(std::string directive : globals::directives) {
        if(directive == value) {
            return true;
        }
    }
    return false;
}

std::string Lexer::collect_identifier(std::string line) {
    std::string collect;
    collect.push_back(line[line_index]);
    line_index++;
    while(true) {
        if(!isalnum(line[line_index]) && line[line_index] != '_') {
            break;
        }
        collect.push_back(line[line_index]);
        line_index++;
    }

    return collect;
}

std::string Lexer::collect_string(std::string line) {
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

std::string Lexer::collect_integer(std::string line, bool is_hex) {
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

std::string type_to_string(TokenType type) {
    switch(type) {
        case TokenType::eof: return "eof";
        case TokenType::mnemonic: return "mnemonic";
        case TokenType::integer: return "integer";
        case TokenType::identifier: return "identifier";
        case TokenType::int_literal: return "int_literal";
        case TokenType::char_literal: return "char_literal";
        case TokenType::register_char: return "register";
        case TokenType::string_literal: return "string_literal";
        case TokenType::label: return "label";
        case TokenType::directive: return "directive";
        case TokenType::reference: return "reference";
        case TokenType::array_access: return "array_access";

        default: return "unknown";
    }
}