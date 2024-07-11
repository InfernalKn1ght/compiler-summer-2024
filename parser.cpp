#include "parser.h"

#include <stdexcept>

namespace AST {
Econst Parser::get_const() {
    std::string acc;
    const size_t size = str.size();
    size_t pos = 0;
    while (pos < size && std::isdigit(str[pos])) {
		acc += str[pos];
		pos++;
    }

    if (acc.size() > 0) {
		Econst result(acc);
		return result;
    } else {
		// TODO: исправить костыль с -1
		throw std::invalid_argument("String is not a digit: " + str);
		Econst result(0);
		return result;
    }
};

std::string Parser::get_keyword_or_ident() {
    std::string acc;
    const size_t size = str.size();
    size_t pos = 0;
    while (pos < size && std::isalpha(str[pos])) {
		acc += str[pos];
		pos++;
    }

    if (acc.size() == 0) {
		throw std::invalid_argument("String is not a keyword or ident: " + str);
    }

    return acc;
};

std::string Parser::get_keyword() {
    try {
		std::string result = get_keyword_or_ident();
		if (Parser(result).is_keyword()) {
			return result;
		} else {
			throw std::invalid_argument("String is not a keyword: " + str);
			return "";
		};
    } catch (std::invalid_argument) {
		throw std::invalid_argument("String is not a keyword: " + str);
		return "";
    }
};

std::string Parser::get_ident() {
	//TODO Переписать понятнее
    try {
		std::string result = get_keyword_or_ident();
		if (!Parser(result).is_keyword()) {
			return result;
		} else {
			throw std::invalid_argument("String is not a ident: " + str);
			return "";
		};
    } catch (std::invalid_argument) {
		throw std::invalid_argument("String is not a ident: " + str);
		return "";
    }
};

}  // namespace AST
