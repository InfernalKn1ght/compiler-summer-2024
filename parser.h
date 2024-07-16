#pragma once

#include "types.h"
#include <string>
#include <memory>

namespace AST {
class Parser {
	private:
		const std::string str;
		std::size_t pos;
		const std::size_t size;
		std::string get_keyword_or_ident();
		bool is_keyword(std::string);
		bool is_variable(std::string);
		bool is_whitespace(char);
		void ws();
		const EConst get_const();
		const std::string get_variable();
		const std::string get_keyword();
		const BinaryOperator get_binary_operation();
		const UnaryOperator get_unary_operation();

	public:
		Parser(const std::string& str) : str(str), pos(0), size(str.size()) {};
		std::unique_ptr<Expr> expr();
};
}
