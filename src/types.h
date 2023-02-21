/**
 *
 * @file: types.h
 * @date: 20.02.2023
 */

#ifndef IPKLIB_TYPES_H
#define IPKLIB_TYPES_H

namespace IPK::AaaS {
    typedef enum {
        END_OF_FILE = 1 << 0,

        LEFT_PARENTHESIS = 1 << 1,
        RIGHT_PARENTHESIS = 1 << 2,

        NUMBER = 1 << 3,

        PLUS = 1 << 4,
        MINUS = 1 << 5,
        MULTIPLY = 1 << 6,
        DIVIDE = 1 << 7,

        OPERATOR = PLUS | MINUS | MULTIPLY | DIVIDE,
    } TOKEN_TYPE;

    inline TOKEN_TYPE operator|(TOKEN_TYPE a, TOKEN_TYPE b) {
        return static_cast<TOKEN_TYPE>(static_cast<int>(a) | static_cast<int>(b));
    }
}// namespace IPK::AaaS

#endif// IPKLIB_TYPES_H
