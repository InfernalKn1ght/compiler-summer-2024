#include "types.h"
#include <memory>
#include <iostream>

namespace AST {
	/**
	 * @brief ВАЖНО - вызов функции полностью стирает дерево
	 *	
	 * @param expr Корень AST дерева
	 */
	void pretty_print(std::unique_ptr<Expr> expr){
	if (AST::EConst* econ = dynamic_cast<AST::EConst*>(expr.get())){
		std::cout << econ->val;
	} else if (AST::EBinOp* ebinop = dynamic_cast<AST::EBinOp*>(expr.get())){
		pretty_print(std::move(ebinop->left));
		std::cout << ' ' << char(ebinop->op) << ' ';
		pretty_print(std::move(ebinop->right));
	} else if (AST::EUnaryOp* eunop = dynamic_cast<AST::EUnaryOp*>(expr.get())){
		pretty_print(std::move(eunop->expression));
		std::cout << char(eunop->op);
	}
	};
}
