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

//TODO
// Надо бы сделать это дело расширяемым, чтобы не менять несколько объектов при добавлении нового оператора
// А если оператор из нескольких символов ?
enum Operator { PLUS='+', MINUS='-', MULTIPLICATION='*' };

class EBinop : public Expr {
	public:
		EBinop(Expr* left, Expr* right, Operator op)
		: left(left), right(right), op(op) {};
		Expr* left;
		Expr* right;
		Operator op;
};
}
