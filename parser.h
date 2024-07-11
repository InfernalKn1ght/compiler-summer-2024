#pragma once

#include "ast.h"
#include <cstddef>
#include <string>

namespace AST {
class Parser {
	private:
		const std::string str;
		std::size_t pos;
		bool is_keyword();
		std::string get_keyword_or_ident();
		void ws();

	public:
		Parser(const std::string& str) : str(str) {};
		Econst get_const();

		const std::string get_ident();
		const std::string get_keyword();
};
}
