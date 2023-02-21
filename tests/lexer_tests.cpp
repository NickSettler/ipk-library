/**
 * IPK Lexer tests
 *
 * @file: lexer_tests.cpp
 * @date: 20.02.2023
 */

#include <gtest/gtest.h>

#include "../src/types.h"
#include "../src/lexer.h"
#include "../src/lexer.cpp"

namespace IPK::tests {
    namespace {
        class LexerTests : public ::testing::Test {
        protected:
            std::vector<AaaS::LexicalToken> actual_tokens;
            std::istringstream input_stream;

        public:
            LexerTests() {
                actual_tokens = std::vector<AaaS::LexicalToken>();
                input_stream = std::istringstream();
            }

            void TearDown() override {
                actual_tokens.clear();
                input_stream.clear();
            }

            void ProcessInput(const std::string &input, const std::vector<AaaS::LexicalToken> &expected_tokens) {
                input_stream = std::istringstream(input);
                AaaS::Lexer lexer(input_stream);
                auto *token = lexer.next_token();

                while (token != nullptr && token->get_type() != AaaS::TOKEN_TYPE::END_OF_FILE) {
                    actual_tokens.push_back(*token);
                    token = lexer.next_token();
                }

                EXPECT_EQ(actual_tokens.size(), expected_tokens.size()) << "Input: " << input;
                
                for (auto expected_token: expected_tokens) {
                    auto actual_token = actual_tokens.front();

                    if (expected_token.get_type())
                        EXPECT_EQ(actual_token.get_type(), expected_token.get_type()) << "Input: " << input;

                    EXPECT_EQ(actual_token.get_value(), expected_token.get_value()) << "Input: " << input;

                    actual_tokens.erase(actual_tokens.begin());
                }
            }
        };

        TEST_F(LexerTests, EmptyInput) {
            ProcessInput("", {});

            ProcessInput("    ", {});

            ProcessInput("    \n   ", {});

            ProcessInput("    \n\t\n   ", {});
        }

        TEST_F(LexerTests, Brackets) {
            ProcessInput("((", {AaaS::LexicalToken("(", AaaS::TOKEN_TYPE::LEFT_PARENTHESIS),
                                AaaS::LexicalToken("(", AaaS::TOKEN_TYPE::LEFT_PARENTHESIS)});

            ProcessInput("()", {AaaS::LexicalToken("(", AaaS::TOKEN_TYPE::LEFT_PARENTHESIS),
                                AaaS::LexicalToken(")", AaaS::TOKEN_TYPE::RIGHT_PARENTHESIS)});

            ProcessInput(")(", {AaaS::LexicalToken(")", AaaS::TOKEN_TYPE::RIGHT_PARENTHESIS),
                                AaaS::LexicalToken("(", AaaS::TOKEN_TYPE::LEFT_PARENTHESIS)});

            ProcessInput("))", {AaaS::LexicalToken(")", AaaS::TOKEN_TYPE::RIGHT_PARENTHESIS),
                                AaaS::LexicalToken(")", AaaS::TOKEN_TYPE::RIGHT_PARENTHESIS)});
        }

        TEST_F(LexerTests, Operators) {
            ProcessInput("+-*/", {AaaS::LexicalToken("+", AaaS::TOKEN_TYPE::PLUS),
                                  AaaS::LexicalToken("-", AaaS::TOKEN_TYPE::MINUS),
                                  AaaS::LexicalToken("*", AaaS::TOKEN_TYPE::MULTIPLY),
                                  AaaS::LexicalToken("/", AaaS::TOKEN_TYPE::DIVIDE)});
        }

        TEST_F(LexerTests, Numbers) {
            ProcessInput("123", {AaaS::LexicalToken("123", AaaS::TOKEN_TYPE::NUMBER)});

            ProcessInput("12 34", {AaaS::LexicalToken("12", AaaS::TOKEN_TYPE::NUMBER),
                                   AaaS::LexicalToken("34", AaaS::TOKEN_TYPE::NUMBER)});
        }

        TEST_F(LexerTests, Query) {
            ProcessInput("(+ 10 20)", {AaaS::LexicalToken("(", AaaS::TOKEN_TYPE::LEFT_PARENTHESIS),
                                       AaaS::LexicalToken("+", AaaS::TOKEN_TYPE::PLUS),
                                       AaaS::LexicalToken("10", AaaS::TOKEN_TYPE::NUMBER),
                                       AaaS::LexicalToken("20", AaaS::TOKEN_TYPE::NUMBER),
                                       AaaS::LexicalToken(")", AaaS::TOKEN_TYPE::RIGHT_PARENTHESIS)});
        }
    }// namespace
}// namespace IPK::tests