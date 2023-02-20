/**
 * IPK Input Lexer
 *
 * @file: lexer.cpp
 * @date: 20.02.2023
 */

#include "lexer.h"

namespace IPK::AaaS {
    Lexer::Lexer(std::istream &input) : input(input) {}

    Lexer::~Lexer() {
        this->input.clear();
        this->input.seekg(0, std::ios::beg);
    }

    LexicalToken *Lexer::next_token() {
        std::string token_string;

        while (true) {
            current_char = (char) this->input.get();

            switch (current_state) {
                case E_LEXER_STATE_START:
                    switch (current_char) {
                        case ' ':
                        case '\n':
                        case '\t':
                            break;
                        case EOF:
                        case '\0':
                            return new LexicalToken("", TOKEN_TYPE::END_OF_FILE);
                        case '(':
                            return new LexicalToken("(", TOKEN_TYPE::LEFT_PARENTHESIS);
                        case ')':
                            return new LexicalToken(")", TOKEN_TYPE::RIGHT_PARENTHESIS);
                        case '+':
                            return new LexicalToken("+", TOKEN_TYPE::PLUS);
                        case '-':
                            return new LexicalToken("-", TOKEN_TYPE::MINUS);
                        case '*':
                            return new LexicalToken("*", TOKEN_TYPE::MULTIPLY);
                        case '/':
                            return new LexicalToken("/", TOKEN_TYPE::DIVIDE);
                        default:
                            if (isdigit(current_char)) {
                                token_string += current_char;
                                current_state = E_LEXER_STATE_NUMBER;
                            } else {
                                throw std::runtime_error("Invalid character");
                            }
                    }
                    break;
                case E_LEXER_STATE_NUMBER:
                    if (isdigit(current_char)) {
                        token_string += current_char;
                    } else {
                        this->input.unget();
                        current_state = E_LEXER_STATE_START;
                        return new LexicalToken(token_string, TOKEN_TYPE::NUMBER);
                    }
                    break;
            }
        }
    }

    LexicalToken::LexicalToken(std::string value, TOKEN_TYPE type)
        : value(std::move(value)), type(type) {}

    LexicalToken::~LexicalToken() { this->value.clear(); }

    std::string LexicalToken::get_value() { return this->value; }

    TOKEN_TYPE AaaS::LexicalToken::get_type() { return this->type; }
}// namespace IPK
