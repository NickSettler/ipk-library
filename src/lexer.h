/**
 * IPK Input Lexer
 * @file: lexer.h
 * @date: 20.02.2023
 */

#ifndef IPKLIB_LEXER_H
#define IPKLIB_LEXER_H

#include <istream>
#include "types.h"

namespace IPK::AaaS {
    typedef enum { E_LEXER_STATE_START, E_LEXER_STATE_NUMBER } E_LEXER_STATE;

    class LexicalToken {
    private:
        std::string value;
        TOKEN_TYPE type;

    public:
        LexicalToken(std::string value, TOKEN_TYPE type);

        ~LexicalToken();

        std::string get_value();

        TOKEN_TYPE get_type();
    };

    class Lexer {
    private:
        std::istream &input;

        char current_char;

        E_LEXER_STATE current_state = E_LEXER_STATE_START;

    public:
        explicit Lexer(std::istream &input);

        ~Lexer();

        LexicalToken *next_token();
    };
}// namespace IPK::AaaS

#endif// IPKLIB_LEXER_H
