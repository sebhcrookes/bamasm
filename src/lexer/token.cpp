#include "token.h"

Token::Token(std::string filename, std::string contents, TokenType type, int line_num) {
    this->filename = filename;
    this->contents = contents;
    this->type     = type;
    this->line_num = line_num;
}