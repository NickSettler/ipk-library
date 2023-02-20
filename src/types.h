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
    } TOKEN_TYPE;
}// namespace IPK::AaaS

#endif// IPKLIB_TYPES_H
