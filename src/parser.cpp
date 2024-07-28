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

	const std::set<std::string> Parser::keywords{
		"if",
		"then",
		"else",
		"fi",
		"while",
		"do",
		"done"
	};
	

    bool Parser::is_keyword(std::string str) {
		return (keywords.count(str) > 0);
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
		auto init_pos = pos;
        try {
            std::string result = get_keyword_or_ident();
            if (is_keyword(result))
                return result;
        } catch (std::invalid_argument) {}
		pos = init_pos;
		throw std::invalid_argument("String is not a keyword: " + str.substr(pos));
    };

    const std::string Parser::get_variable() {
		auto init_pos = pos;
        try {
            std::string result = get_keyword_or_ident();
            if (is_variable(result))
                return result;
        } catch (std::invalid_argument) {}
		pos = init_pos;
		throw std::invalid_argument("String is not a variable: " + str.substr(pos));
    };

    void Parser::ws() {
        while (pos < size && is_whitespace(str[pos])) {
            ++pos;
        }
    }

	const BinaryOperator Parser::get_binary_operation() {
		ws();
		char symbol = str[pos];
		switch (symbol) {
			case '+':
			case '-':
			case '*':
            case '<':
            case '>':
            case '=':
				pos++;
				return BinaryOperator(symbol);
			default:
				break;
		}

		if (str.substr(pos, 2) == ":=") {
			pos += 2;
			return ASSIGMENT;
		}

        if (str.substr(pos, 3) == "and") {
            pos += 3;
            return AND;
        }

        if (str.substr(pos, 2) == "or") {
            pos += 2;
            return OR;
        }

		throw std::invalid_argument("String is not an operator: " + str.substr(pos));
	}

    const UnaryOperator Parser::get_unary_operation() {
        ws();
        char symbol = str[pos];
        switch (symbol) {
        case '!':
            pos++;
            return UnaryOperator(symbol);
        default:
            throw std::invalid_argument("Char is not a operator " + str.substr(pos, 1));
        }
    }

    const Bracket Parser::get_bracket() {
        ws();
        char symbol = str[pos];
        switch (symbol) {
        case '(':
		case ')':
            pos++;
            return Bracket(symbol);
        default:
            throw std::invalid_argument("Char is not a bracket " + str.substr(pos, 1));
        }
    }

} // namespace AST
