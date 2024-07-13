#pragma once

#include "ast.h"
#include <cstddef>
#include <string>

namespace AST {

bool is_keyword(std::string);
bool is_ident(std::string);

class Parser {
	private:
		const std::string str;
		std::size_t pos;
		std::string get_keyword_or_ident();
		void ws();

	public:
		Parser(const std::string& str) : str(str), pos(0) {};
		Econst get_const();

		const std::string get_ident();
		const std::string get_keyword();
		const Operator get_operator();
};
}
