/**
 *
 * @file: parser.h
 * @date: 20.02.2023
 */

#ifndef IPKLIB_PARSER_H
#define IPKLIB_PARSER_H

#include "lexer.h"

namespace IPK::AaaS {
    typedef enum {
        PRE_ORDER,
        IN_ORDER,
        POST_ORDER,
    } TreeTraversalType;

    class SyntaxTree {
    private:
        TOKEN_TYPE type;
        std::string value;

        SyntaxTree *left;
        SyntaxTree *right;

    public:
        SyntaxTree(TOKEN_TYPE type, std::string value);
        SyntaxTree(TOKEN_TYPE type, std::string value, SyntaxTree *left, SyntaxTree *right);

        ~SyntaxTree();

        void traverse(std::function<void(SyntaxTree *)> &callback, TreeTraversalType type);

        void set_value(std::string value);

        std::string get_value();

        void set_type(TOKEN_TYPE type);

        TOKEN_TYPE get_type();

        SyntaxTree *get_left();

        SyntaxTree *get_right();
    };

    class Parser {
    private:
        LexicalToken *current_token;

        std::function<LexicalToken *()> &lexer_func;

        void expect_token(TOKEN_TYPE type);

        SyntaxTree *expr();

    public:
        explicit Parser(std::function<LexicalToken *()> &lexer_func);

        ~Parser();

        SyntaxTree *parse();
    };
}// namespace IPK::AaaS

#endif// IPKLIB_PARSER_H
