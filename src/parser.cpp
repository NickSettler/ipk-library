/**
 * IPK Input Parser
 *
 * @file: parser.cpp
 * @date: 20.02.2023
 */

#include "parser.h"

std::map<IPK::AaaS::TOKEN_TYPE, std::string> TOKEN_TYPE_MAP = {
        {IPK::AaaS::TOKEN_TYPE::END_OF_FILE, "END_OF_FILE"},

        {IPK::AaaS::TOKEN_TYPE::LEFT_PARENTHESIS, "LEFT_PARENTHESIS"},
        {IPK::AaaS::TOKEN_TYPE::RIGHT_PARENTHESIS, "RIGHT_PARENTHESIS"},

        {IPK::AaaS::TOKEN_TYPE::NUMBER, "NUMBER"},

        {IPK::AaaS::TOKEN_TYPE::PLUS, "PLUS"},
        {IPK::AaaS::TOKEN_TYPE::MINUS, "MINUS"},
        {IPK::AaaS::TOKEN_TYPE::MULTIPLY, "MULTIPLY"},
        {IPK::AaaS::TOKEN_TYPE::DIVIDE, "DIVIDE"},
        {IPK::AaaS::TOKEN_TYPE::OPERATOR, "OPERATOR"},
};

IPK::AaaS::SyntaxException::SyntaxException(std::string message) { this->message = std::move(message); }

const char *IPK::AaaS::SyntaxException::what() const noexcept { return message.c_str(); }

IPK::AaaS::SyntaxTree::SyntaxTree(IPK::AaaS::TOKEN_TYPE type, std::string value) : type(type), value(std::move(value)) {
    left = nullptr;
    right = nullptr;
}

IPK::AaaS::SyntaxTree::SyntaxTree(IPK::AaaS::TOKEN_TYPE type, std::string value, IPK::AaaS::SyntaxTree *left,
                                  IPK::AaaS::SyntaxTree *right)
    : type(type), value(std::move(value)), left(left), right(right) {}

IPK::AaaS::SyntaxTree::~SyntaxTree() {
    delete left;
    delete right;
}

void IPK::AaaS::SyntaxTree::traverse(std::function<void(SyntaxTree *)> &callback, IPK::AaaS::TreeTraversalType type) {
    if (this == nullptr) return;

    if (type == TreeTraversalType::PRE_ORDER) callback(this);
    if (left != nullptr) left->traverse(callback, type);
    if (type == TreeTraversalType::IN_ORDER) callback(this);
    if (right != nullptr) right->traverse(callback, type);
    if (type == TreeTraversalType::POST_ORDER) callback(this);
}

void IPK::AaaS::SyntaxTree::set_value(std::string value) { this->value = std::move(value); }

std::string IPK::AaaS::SyntaxTree::get_value() { return value; }

void IPK::AaaS::SyntaxTree::set_type(IPK::AaaS::TOKEN_TYPE type) { this->type = type; }

IPK::AaaS::TOKEN_TYPE IPK::AaaS::SyntaxTree::get_type() { return type; }

IPK::AaaS::SyntaxTree *IPK::AaaS::SyntaxTree::get_left() { return left; }

IPK::AaaS::SyntaxTree *IPK::AaaS::SyntaxTree::get_right() { return right; }

IPK::AaaS::Parser::Parser(std::function<LexicalToken *()> &lexer_func) : lexer_func(lexer_func) {
    current_token = lexer_func();
}

IPK::AaaS::Parser::~Parser() { delete current_token; }

void IPK::AaaS::Parser::expect_token(IPK::AaaS::TOKEN_TYPE type) {
    if (!(current_token->get_type() & type)) {
        std::stringstream ss;
        ss << "Unexpected token. Expected " << IPK::AaaS::ParserUtils::token_type_to_string(type);
        throw SyntaxException(ss.str());
    }

    current_token = lexer_func();
}

IPK::AaaS::SyntaxTree *IPK::AaaS::Parser::expr() {
    bool isNumber = current_token->get_type() == TOKEN_TYPE::NUMBER;
    bool isExpression = current_token->get_type() == TOKEN_TYPE::LEFT_PARENTHESIS;

    if (!isNumber && !isExpression) throw SyntaxException("Unexpected token. Expected number or expression");

    if (isNumber) {
        auto tree = new SyntaxTree(TOKEN_TYPE::NUMBER, current_token->get_value());
        current_token = lexer_func();
        return tree;
    }

    expect_token(TOKEN_TYPE::LEFT_PARENTHESIS);

    if (!IPK::AaaS::ParserUtils::is_operator(current_token->get_type())) {
        throw SyntaxException("Unexpected token. Expected operator");
    }

    TOKEN_TYPE operator_type = current_token->get_type();

    current_token = lexer_func();

    SyntaxTree *left = expr();
    SyntaxTree *right = expr();

    expect_token(TOKEN_TYPE::RIGHT_PARENTHESIS);

    return new SyntaxTree(operator_type, "", left, right);
}

IPK::AaaS::SyntaxTree *IPK::AaaS::Parser::build_tree() {
    SyntaxTree *tree = nullptr;

    if (current_token->get_type() == TOKEN_TYPE::END_OF_FILE) { return tree; }

    if (current_token->get_type() != TOKEN_TYPE::LEFT_PARENTHESIS)
        throw SyntaxException("Unexpected token. Expected (");

    while (current_token->get_type() != TOKEN_TYPE::END_OF_FILE) { tree = expr(); }

    return tree;
}
bool IPK::AaaS::ParserUtils::is_operator(IPK::AaaS::TOKEN_TYPE type) {
    return type & (TOKEN_TYPE::PLUS | TOKEN_TYPE::MINUS | TOKEN_TYPE::MULTIPLY | TOKEN_TYPE::DIVIDE);
}

bool IPK::AaaS::ParserUtils::is_valid_input(const std::string &input) {
    std::istream *stream = new std::istringstream(input);

    return is_valid_input(*stream);
}

bool IPK::AaaS::ParserUtils::is_valid_input(std::istream &input) {
    auto lexer = new Lexer(input);

    std::function<IPK::AaaS::LexicalToken *()> parser_func = [lexer]() { return lexer->next_token(); };

    auto parser = new Parser(parser_func);

    try {
        parser->build_tree();
    } catch (IPK::AaaS::SyntaxException &e) {
        delete lexer;
        delete parser;
        return false;
    }

    delete lexer;
    delete parser;
    return true;
}

const char *IPK::AaaS::ParserUtils::token_type_to_string(IPK::AaaS::TOKEN_TYPE type) {
    if (!TOKEN_TYPE_MAP.contains(type)) throw IPK::AaaS::SyntaxException("Unknown token type");

    return TOKEN_TYPE_MAP.at(type).c_str();
}