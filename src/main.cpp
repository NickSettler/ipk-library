#include <iostream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

int main() {
    std::string input = "(+ 100 (* 20 (* 20 30)))";
    std::istringstream input_stream(input);

    auto lexer = new IPK::AaaS::Lexer(input_stream);

    std::function<IPK::AaaS::LexicalToken *()> parser_func = [lexer]() { return lexer->next_token(); };

    auto *parser = new IPK::AaaS::Parser(parser_func);
    IPK::AaaS::SyntaxTree *tree = parser->parse();

    std::function<void(IPK::AaaS::SyntaxTree *)> callback = [](IPK::AaaS::SyntaxTree *node) {
        if (node->get_type() & (IPK::AaaS::TOKEN_TYPE::PLUS | IPK::AaaS::TOKEN_TYPE::MINUS |
                                IPK::AaaS::TOKEN_TYPE::MULTIPLY | IPK::AaaS::TOKEN_TYPE::DIVIDE)) {
            int left_number = std::stoi(node->get_left()->get_value());
            int right_number = std::stoi(node->get_right()->get_value());

            int result = 0;
            switch (node->get_type()) {
                case IPK::AaaS::TOKEN_TYPE::PLUS:
                    result = left_number + right_number;
                    break;
                case IPK::AaaS::TOKEN_TYPE::MINUS:
                    result = left_number - right_number;
                    break;
                case IPK::AaaS::TOKEN_TYPE::MULTIPLY:
                    result = left_number * right_number;
                    break;
                case IPK::AaaS::TOKEN_TYPE::DIVIDE:
                    result = left_number / right_number;
                    break;
                default:
                    break;
            }

            node->set_value(std::to_string(result));
            node->set_type(IPK::AaaS::TOKEN_TYPE::NUMBER);
        }
    };

    tree->traverse(callback, IPK::AaaS::TreeTraversalType::POST_ORDER);

    printf("Result: %s\n", tree->get_value().c_str());

    return 0;
}
