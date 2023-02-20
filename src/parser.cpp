/**
 *
 * @file: parser.cpp
 * @date: 20.02.2023
 */

#include "parser.h"

IPK::AaaS::Parser::Parser(std::function<LexicalToken *()> &lexer_func) : lexer_func(lexer_func) {
    current_token = lexer_func();
}

IPK::AaaS::Parser::~Parser() = default;

void IPK::AaaS::Parser::expect_token(IPK::AaaS::LEXICAL_TOKEN_TYPE type) {
    if (current_token->get_type() != type) { throw std::runtime_error("Unexpected token"); }

    current_token = lexer_func();
}

IPK::AaaS::SyntaxTree *IPK::AaaS::Parser::expr() {
    bool isNumber = current_token->get_type() == LEXICAL_TOKEN_TYPE::NUMBER;
    bool isExpression = current_token->get_type() == LEXICAL_TOKEN_TYPE::LEFT_PARENTHESIS;

    if (!isNumber && !isExpression) throw std::runtime_error("Unexpected token");

    if (isNumber) {
        auto tree = new SyntaxTree(LEXICAL_TOKEN_TYPE::NUMBER, current_token->get_value());
        current_token = lexer_func();
        return tree;
    }

    expect_token(LEXICAL_TOKEN_TYPE::LEFT_PARENTHESIS);

    if (!(current_token->get_type() & (LEXICAL_TOKEN_TYPE::PLUS | LEXICAL_TOKEN_TYPE::MINUS |
                                       LEXICAL_TOKEN_TYPE::MULTIPLY | LEXICAL_TOKEN_TYPE::DIVIDE))) {
        throw std::runtime_error("Unexpected token. Expected operator");
    }

    LEXICAL_TOKEN_TYPE operator_type = current_token->get_type();

    current_token = lexer_func();

    SyntaxTree *left = expr();
    SyntaxTree *right = expr();

    expect_token(LEXICAL_TOKEN_TYPE::RIGHT_PARENTHESIS);

    return new SyntaxTree(operator_type, "", left, right);
}
IPK::AaaS::SyntaxTree *IPK::AaaS::Parser::parse() {
    SyntaxTree *tree = nullptr;
    while (current_token->get_type() != LEXICAL_TOKEN_TYPE::END_OF_FILE) tree = expr();

    return tree;
}

IPK::AaaS::SyntaxTree::SyntaxTree(IPK::AaaS::LEXICAL_TOKEN_TYPE type, std::string value)
    : type(type), value(std::move(value)) {
    left = nullptr;
    right = nullptr;
}

IPK::AaaS::SyntaxTree::SyntaxTree(IPK::AaaS::LEXICAL_TOKEN_TYPE type, std::string value, IPK::AaaS::SyntaxTree *left,
                                  IPK::AaaS::SyntaxTree *right)
    : type(type), value(std::move(value)), left(left), right(right) {}

IPK::AaaS::SyntaxTree::~SyntaxTree() {
    delete left;
    delete right;
}

void IPK::AaaS::SyntaxTree::traverse(std::function<void(SyntaxTree *)> &callback, IPK::AaaS::TreeTraversalType type) {
    if (type == TreeTraversalType::PRE_ORDER) callback(this);
    if (left != nullptr) left->traverse(callback, type);
    if (type == TreeTraversalType::IN_ORDER) callback(this);
    if (right != nullptr) right->traverse(callback, type);
    if (type == TreeTraversalType::POST_ORDER) callback(this);
}

void IPK::AaaS::SyntaxTree::set_value(std::string value) { this->value = std::move(value); }

std::string IPK::AaaS::SyntaxTree::get_value() { return value; }

void IPK::AaaS::SyntaxTree::set_type(IPK::AaaS::LEXICAL_TOKEN_TYPE type) { this->type = type; }

IPK::AaaS::LEXICAL_TOKEN_TYPE IPK::AaaS::SyntaxTree::get_type() { return type; }

IPK::AaaS::SyntaxTree *IPK::AaaS::SyntaxTree::get_left() { return left; }

IPK::AaaS::SyntaxTree *IPK::AaaS::SyntaxTree::get_right() { return right; }
