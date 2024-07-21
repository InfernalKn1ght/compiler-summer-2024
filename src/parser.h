#pragma once

#include "types.h"
#include <cstddef>
#include <string>
#include <memory>
#include <set>

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
		static const std::set<std::string> keywords;

	public:
		Parser(const std::string& str) : str(str), pos(0), size(str.size()) {};
		const EConst get_const();
		const EVariable get_variable();
		const std::string get_keyword();
		const BinaryOperator get_binary_operation();
		const UnaryOperator get_unary_operation();
		const Bracket get_bracket();

		void print() const {std::cout << str << " <-- str; " << pos << " <-- pos; " << str.substr(pos) << " <-- substring;\n\n";};
		bool eof() { ws(); return pos==size; };
		std::size_t get_pos() const { return pos; };
		void set_pos(std::size_t new_pos){ pos=new_pos; };
};
}
