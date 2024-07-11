#pragma once

#include "ast.h"
#include <string>

namespace AST {
class Parser {
	private:
		const std::string str;
		bool is_keyword();
		std::string get_keyword_or_ident();

	public:
		Parser(const std::string& str) : str(str) {};
		Econst get_const();

		std::string get_ident();
		std::string get_keyword();
};
}
