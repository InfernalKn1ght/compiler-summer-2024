#include "parser.h"
#include "types.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace AST {
    bool Parser::is_whitespace(char c) {
        switch (c) {
        case ' ':
            return 1;
            break;
        case '\n':
            return 1;
            break;
        case '\t':
            return 1;
            break;
        default:
            return 0;
            break;
        }
    };

    bool Parser::is_keyword(std::string str) {
        return false;
    };

    bool Parser::is_variable(std::string str) {
        return (!is_keyword(str));
    };

    const EConst Parser::get_const() {
        ws();
        std::string acc;
        while (pos < size && std::isdigit(str[pos])) {
            acc += str[pos];
            pos++;
        }

        if (acc.size() == 0) {
            throw std::invalid_argument("String is not a digit: " + str.substr(pos));
        }

        EConst result(acc);
        return result;
    };

    std::string Parser::get_keyword_or_ident() {
        ws();
        std::string acc;
        while (pos < size && std::isalpha(str[pos])) {
            acc += str[pos];
            pos++;
        }

        if (acc.size() == 0) {
            throw std::invalid_argument("String is not a keyword or ident: " + str.substr(pos));
        }

        return acc;
    };

    const std::string Parser::get_keyword() {
        try {
            std::string result = get_keyword_or_ident();
            if (is_keyword(result))
                return result;
            throw std::invalid_argument("String is not a keyword: " + str.substr(pos));
        } catch (std::invalid_argument) {
            throw std::invalid_argument("String is not a keyword: " + str.substr(pos));
        }
    };

    const std::string Parser::get_variable() {
        try {
            std::string result = get_keyword_or_ident();
            if (is_variable(result))
                return result;
            throw std::invalid_argument("String is not a variable: " + str.substr(pos));
        } catch (std::invalid_argument) {
            throw std::invalid_argument("String is not a variable: " + str.substr(pos));
        }
    };

    void Parser::ws() {
        while (pos < size && is_whitespace(str[pos])) {
            ++pos;
        }
    }

    const BinaryOperator Parser::get_binary_operation() {
        ws();
        // TODO - А если оператор не char а string
        char symbol = str[pos];
        switch (symbol) {
        case '+':
        case '-':
        case '*':
            pos++;
            return BinaryOperator(symbol);
        default:
            throw std::invalid_argument("Char is not a operator " + str.substr(pos, 1));
        }
    }

    const UnaryOperator Parser::get_unary_operation() {
        ws();
        // TODO - А если оператор не char а string
        char symbol = str[pos];
        switch (symbol) {
        case '!':
            pos++;
            return UnaryOperator(symbol);
        default:
            throw std::invalid_argument("Char is not a operator " + str.substr(pos, 1));
        }
    }

} // namespace AST
