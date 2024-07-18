#include "ast_builder.h"
#include "parser.h"
#include "types.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace AST {
    std::unique_ptr<Expr> AstBuilder::prod() {
        std::unique_ptr<Expr> head;
        try {
            head = std::make_unique<EConst>(p.get_const());
        } catch (std::invalid_argument) {}

		try {
			head = std::make_unique<EVariable>(p.get_variable());
		} catch (std::invalid_argument){}

		if (head.get() == nullptr)
            throw std::logic_error("Invalid syntax");

        const std::size_t init_pos = p.get_pos();

        try {
            BinaryOperator op = p.get_binary_operation();

            if (op == MULTIPLICATION) {
                std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
                    std::move(head),
                    std::move(prod()),
                    op);
                return std::move(new_root);
            }

        } catch (std::invalid_argument &e) {}
        p.set_pos(init_pos);
        return std::move(head);
    }

    std::unique_ptr<Expr> AstBuilder::expr() {
        // try {
        //     UnaryOperator op = p.get_unary_operation();
        // 	std::unique_ptr<EUnaryOp> new_head = std::make_unique<EUnaryOp>(
        // 		std::move(head),
        // 		op
        // 	);
        // 	head = std::move(new_head);
        // } catch (std::invalid_argument &e) {}

        std::unique_ptr<Expr> result;
        try {
            result = prod();
        } catch (std::invalid_argument &e) {}

		const std::size_t init_pos = p.get_pos();
        try {
            BinaryOperator op = p.get_binary_operation();
            std::unique_ptr<EBinOp> new_root;

            if (op == PLUS || op == MINUS) {
                new_root = std::make_unique<EBinOp>(
                    std::move(result),
                    std::move(expr()),
                    op);
                return std::move(new_root);
            }
        } catch (std::invalid_argument &e) {}
		p.set_pos(init_pos);
        return std::move(result);
    }

    std::unique_ptr<EBinOp> AstBuilder::stmt() {
        const std::size_t init_pos = p.get_pos();

        try {
			std::unique_ptr<EVariable> ident = std::make_unique<EVariable>(p.get_variable());
			BinaryOperator op;
			try {
				op = p.get_binary_operation(); 
			} catch (std::invalid_argument) {}
			if (op != ASSIGMENT){
				throw  std::logic_error("Invalid syntax");
			}
			std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
				std::move(ident),
				std::move(expr()),
				op
			);
			return std::move(new_root);
        } catch (std::invalid_argument) {}
        p.set_pos(init_pos);

        try {
            std::unique_ptr<EKeyWord> key_word = std::make_unique<EKeyWord>(p.get_keyword());
            if (key_word->word == "while") {
                std::unique_ptr<Expr> result = expr();
                std::unique_ptr<EKeyWord> key_word_do;

                try {
                    std::unique_ptr<EKeyWord> key_word_do = std::make_unique<EKeyWord>(p.get_keyword());
                } catch (std::invalid_argument) {
                }
                if (key_word_do->word != "do") {
                    throw std::logic_error("Invalid syntax");
                }
                std::unique_ptr<Stmt> body_stmts = stmts();

                try {
                    key_word_do = std::make_unique<EKeyWord>(p.get_keyword());
                } catch (std::invalid_argument) {
                }
                if (key_word_do->word != "done") {
                    throw std::logic_error("Invalid syntax");
                }

                std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
                    std::move(result),
                    std::move(body_stmts),
                    DO);
                return std::move(new_root);
            }
        } catch (std::invalid_argument) {}
		p.set_pos(init_pos);

        throw std::logic_error("Invalid syntax");
    }

    std::unique_ptr<Stmt> AstBuilder::stmts() {
		std::unique_ptr<Stmt> stmt_list;
        try {
            stmt_list = std::make_unique<Stmt>(
				std::move(stmt()),
				std::move(stmts())
			);
        } catch (std::invalid_argument) {}
		return stmt_list;
    }

}
