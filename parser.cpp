#include "parser.h"
#include "ast.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

namespace {
bool is_whitespace(char c) {
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
}
}

namespace AST {
Econst Parser::get_const(){
	ws();
    std::string acc;
    const size_t size = str.size();
    while (pos < size && std::isdigit(str[pos])) {
		acc += str[pos];
		pos++;
    }

    if (acc.size() > 0) {
		Econst result(acc);
		return result;
    } else {
		throw std::invalid_argument("String is not a digit: " + str.substr(pos));
    }
};

std::string Parser::get_keyword_or_ident(){
	ws();
    std::string acc;
    const size_t size = str.size();
    while (pos < size && std::isalpha(str[pos])) {
		acc += str[pos];
		pos++;
    }

    if (acc.size() == 0) {
		throw std::invalid_argument("String is not a keyword or ident: " + str.substr(pos));
    }

    return acc;
};

const std::string Parser::get_keyword(){
    try {
		std::string result = get_keyword_or_ident();
		if (Parser(result).is_keyword()) {
			return result;
		} else {
			throw std::invalid_argument("String is not a keyword: " + str);
		};
    } catch (std::invalid_argument) {
		throw std::invalid_argument("String is not a keyword: " + str);
    }
};

const std::string Parser::get_ident() {
	//TODO Переписать понятнее
    try {
		std::string result = get_keyword_or_ident();
		if (!Parser(result).is_keyword()) {
			return result;
		} else {
			throw std::invalid_argument("String is not a ident: " + str);
		};
    } catch (std::invalid_argument) {
		throw std::invalid_argument("String is not a ident: " + str);
    }
};

void Parser::ws() {
    while (pos < str.size() && is_whitespace(str[pos])) {
        ++pos;
    }
}

bool Parser::is_keyword(){
	return false;
}

const Operator Parser::get_operator(){
	ws();
	//TODO - А если оператор не char а string 
	char symbol = str[pos];
	switch (symbol) {
	case '+':
	case '-':
	case '*':
		pos++;
		return Operator(symbol);
		break;
	default:
		throw std::invalid_argument("Char is not a operator" + str.substr(pos, 1));
		break;
	}
}
}  // namespace AST
