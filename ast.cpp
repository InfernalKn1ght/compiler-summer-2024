#include "ast.h"
#include "parser.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>


namespace AST{
	std::unique_ptr<Expr> AstBuilder::EConst_head(){
		std::unique_ptr<Expr> head;
        try {
			head = std::make_unique<EConst>(p.get_const());
        } catch (std::exception &e) {
            throw std::invalid_argument("Head is not a const");
        }

        try {
            UnaryOperator op = p.get_unary_operation();
			std::unique_ptr<EUnaryOp> new_head = std::make_unique<EUnaryOp>(
				std::move(head),
				op
			);
			head = std::move(new_head);
        } catch (std::invalid_argument &e) {}

        try {
            BinaryOperator op = p.get_binary_operation();
			std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
				std::move(head),
				std::move(get_root()),
				op
			);
			return std::move(new_root);
        } catch (std::invalid_argument &e) {}

		return std::move(head);
	}


	std::unique_ptr<Expr> AstBuilder::get_root(){
		const std::size_t init_pos = p.get_pos();

		try {
			return EConst_head();
		} catch (std::invalid_argument) {}
		p.set_pos(init_pos);	

		
		throw std::invalid_argument("Invalid syntax");
	}


}
