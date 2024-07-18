#include "types.h"
#include <memory>
#include <iostream>

namespace AST {
	void pretty_print(const std::string& prefix, const std::unique_ptr<Expr>& expr, bool isLeft) {
		if (auto econ = dynamic_cast<EConst*>(expr.get())) {
			std::cout << prefix << (isLeft ? "├──" : "└──" ) << econ->val << "\n";
		} else if (auto ebinop = dynamic_cast<EBinOp*>(expr.get())) {
			std::cout << prefix << (isLeft ? "├──" : "└──" ) << char(ebinop->op) << "\n";
			pretty_print(prefix + (isLeft ? "│   " : "    "), ebinop->left, true);
			pretty_print(prefix + (isLeft ? "│   " : "    "), ebinop->right, false);
		} else if (auto econ = dynamic_cast<EVariable*>(expr.get())) {
			std::cout << prefix << (isLeft ? "├──" : "└──" ) << econ->name << "\n";
		} else {
			std::cout << prefix << (isLeft ? "├──" : "└──" ) << "WRONG TYPE" << std::endl;
		}

	}

	void pretty_print(const std::unique_ptr<Expr>& expr) {
		pretty_print("", expr, false);
	}

}
