#pragma once

#include <string>

namespace AST {

class Expr {};

class Econst : public Expr {
	public:
		Econst(int val) : val(val) {};
		Econst(const std::string& str) : val(std::stoi(str)) {};
		int val;
};

enum Operator { ADD, SUM, MUL };

class EBinop : public Expr {
	public:
		EBinop(Expr* left, Expr* right, Operator op)
		: left(left), right(right), op(op) {};
		Expr* left;
		Expr* right;
		Operator op;
};
}
