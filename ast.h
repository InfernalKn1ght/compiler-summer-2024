#pragma once

#include <algorithm>
#include <memory>
#include <string>

namespace AST {

class Expr {
public:
	virtual ~Expr() = default;
};

class EConst : public Expr {
	public:
		EConst(int val) : val(val) {};
		EConst(const std::string& str) : val(std::stoi(str)) {};
		int val;
};

enum BinaryOperator { PLUS='+', MINUS='-', MULTIPLICATION='*' };
enum UnaryOperator { Factorial='!' };

class EOperator : public Expr {
	virtual const std::string record() const = 0; // Функция будет возвращать соответсвующий оператору ассемблерный код
};

class EBinOp : public EOperator {
	public:
		EBinOp(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, BinaryOperator op) :
			left(std::move(left)), right(std::move(right)), op(op) {};
		std::unique_ptr<Expr> left;
		std::unique_ptr<Expr> right;
		BinaryOperator op;

		const std::string record() const;
};

class EUnaryOp : public EOperator {
	public:
		EUnaryOp(std::unique_ptr<Expr> expression, UnaryOperator op) :
			expression(std::move(expression)), op(op) {};
		std::unique_ptr<Expr> expression;
		UnaryOperator op;

		const std::string record() const;
};

void pretty_print(std::unique_ptr<Expr>);

}
