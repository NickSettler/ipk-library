/**
 * IPK Syntax tests
 *
 * @file: syntax_tests.cpp
 * @date: 20.02.2023
 */

#include <gtest/gtest.h>

#include "../src/types.h"
#include "../src/parser.h"
#include "../src/parser.cpp"

namespace IPK::tests {
    namespace {
        class SyntaxTests : public ::testing::Test {
        protected:
            IPK::AaaS::Lexer *lexer{};
            IPK::AaaS::Parser *parser{};
            IPK::AaaS::SyntaxTree *syntax_tree{};

        public:
            void TearDown() override {
                delete lexer;
                delete parser;
                delete syntax_tree;
            }

            void CheckSyntax(const std::string &input, const std::vector<IPK::AaaS::TOKEN_TYPE> &expected_tokens) {
                std::string expected_tokens_str;
                for (auto token: expected_tokens) {
                    expected_tokens_str += std::to_string(token);
                    expected_tokens_str += " ";
                }

                std::istringstream input_stream(input);
                lexer = new IPK::AaaS::Lexer(input_stream);

                std::function<AaaS::LexicalToken *(void)> parser_func = [&]() { return lexer->next_token(); };

                parser = new IPK::AaaS::Parser(parser_func);

                syntax_tree = parser->build_tree();

                std::string actual_tokens;

                std::function<void(IPK::AaaS::SyntaxTree *)> traverse_func =
                        [&actual_tokens](IPK::AaaS::SyntaxTree *node) {
                            actual_tokens += std::to_string(node->get_type());
                            actual_tokens += " ";
                        };

                syntax_tree->traverse(traverse_func, IPK::AaaS::TreeTraversalType ::IN_ORDER);

                EXPECT_STREQ(actual_tokens.c_str(), expected_tokens_str.c_str()) << "Input: " << input;
            }
        };

        TEST_F(SyntaxTests, EmptyInput) {
            CheckSyntax("", {});

            CheckSyntax("    ", {});

            CheckSyntax("  \n\t\n ", {});
        }

        TEST_F(SyntaxTests, SimpleExpression) {
            CheckSyntax("(+ 1 2)", {
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                           IPK::AaaS::TOKEN_TYPE::PLUS,
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                   });

            CheckSyntax("(- 1 2)", {
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                           IPK::AaaS::TOKEN_TYPE::MINUS,
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                   });

            CheckSyntax("(* 1 2)", {
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                           IPK::AaaS::TOKEN_TYPE::MULTIPLY,
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                   });

            CheckSyntax("(/ 1 2)", {
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                           IPK::AaaS::TOKEN_TYPE::DIVIDE,
                                           IPK::AaaS::TOKEN_TYPE::NUMBER,
                                   });
        }

        TEST_F(SyntaxTests, NestedExpresions) {
            CheckSyntax("(+ 1 (+ 2 3))", {
                                                 IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                 IPK::AaaS::TOKEN_TYPE::PLUS,
                                                 IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                 IPK::AaaS::TOKEN_TYPE::PLUS,
                                                 IPK::AaaS::TOKEN_TYPE::NUMBER,
                                         });

            CheckSyntax("(+ 1 (+ 2 (+ 3 4)))", {
                                                       IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                       IPK::AaaS::TOKEN_TYPE::PLUS,
                                                       IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                       IPK::AaaS::TOKEN_TYPE::PLUS,
                                                       IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                       IPK::AaaS::TOKEN_TYPE::PLUS,
                                                       IPK::AaaS::TOKEN_TYPE::NUMBER,
                                               });

            CheckSyntax("(+ 1 (+ 2 (+ 3 (+ 4 5))))", {
                                                             IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                             IPK::AaaS::TOKEN_TYPE::PLUS,
                                                             IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                             IPK::AaaS::TOKEN_TYPE::PLUS,
                                                             IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                             IPK::AaaS::TOKEN_TYPE::PLUS,
                                                             IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                             IPK::AaaS::TOKEN_TYPE::PLUS,
                                                             IPK::AaaS::TOKEN_TYPE::NUMBER,
                                                     });
        }

        TEST_F(SyntaxTests, InvalidInput) {
            EXPECT_THROW(CheckSyntax("1", {}), IPK::AaaS::SyntaxException);

            EXPECT_THROW(CheckSyntax("1 2", {}), IPK::AaaS::SyntaxException);

            EXPECT_THROW(CheckSyntax("- 1 2", {}), IPK::AaaS::SyntaxException);
        }
    }// namespace
}// namespace IPK::tests