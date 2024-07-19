#include "types.h"

#include <iostream>

namespace AST {

	void EConst::print(const std::string& prefix, bool isLeft) const {
		std::cout << prefix << (isLeft ? "├──" : "└──") << val << "\n";
	}

	void EVariable::print(const std::string& prefix, bool isLeft) const {
		std::cout << prefix << (isLeft ? "├──" : "└──") << name << "\n";
	}

	void EBinOp::print(const std::string& prefix, bool isLeft) const {
		auto op_str = [this]() -> std::string{
		switch (op) {
			case PLUS:
			return "+";
			case MINUS:
			return "-";
			case MULTIPLICATION:
			return "*";
			case ASSIGMENT:
			return ":=";
			case WHILE:
			return "While";
			case IF:
			return "If";
			case ELSE:
			return "Else";
			default:
			return "";
		}
		};
		std::cout << prefix << (isLeft ? "├──" : "└──") << op_str() << "\n";
		left->print(prefix + (isLeft ? "│   " : "    "), true);
		right->print(prefix + (isLeft ? "│   " : "    "), false);
	}

	void EUnaryOp::print(const std::string& prefix, bool isLeft) const {
		std::cout << prefix << (isLeft ? "├──" : "└──") << "!"
			  << "\n";
		expression->print(prefix + (isLeft ? "│   " : "    "), true);
	}

	void Stmt::print(const std::string& prefix, bool isLeft) const {
		tree->print(prefix, isLeft);
		if (next != nullptr) {
		next->print(prefix + (isLeft ? "│   " : "    "), true);
		}
	}

}  // namespace AST

