#pragma once

#include <algorithm>
#include <memory>
#include <string>

namespace AST {

class Expr {
public:
	virtual ~Expr() = default;
};

class Econst : public Expr {
	public:
		Econst(int val) : val(val) {};
		Econst(const std::string& str) : val(std::stoi(str)) {};
		int val;
};

enum Operator { PLUS='+', MINUS='-', MULTIPLICATION='*' };

class EBinop : public Expr {
	public:
		EBinop(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, Operator op) :
			left(std::move(left)), right(std::move(right)), op(op) {};
		std::unique_ptr<Expr> left;
		std::unique_ptr<Expr> right;
		Operator op;
};
}
