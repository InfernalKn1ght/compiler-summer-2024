#pragma once

#include "ast.h"
#include <string>

namespace AST {

bool is_keyword(std::string);
bool is_ident(std::string);

class Parser {
	private:
		const std::string str;
		std::size_t pos;
		const std::size_t size;
		std::string get_keyword_or_ident();
		void ws();
		const EConst get_const();
		const std::string get_ident();
		const std::string get_keyword();
		const BinaryOperator get_binary_operator();
		const UnaryOperator get_unary_operator();

	public:
		Parser(const std::string& str) : str(str), pos(0), size(str.size()) {};
		std::unique_ptr<Expr> expr();
};
}
