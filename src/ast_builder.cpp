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
	try {
		BinaryOperator op = p.get_binary_operation();

		if (op == LEFT_BRACKET) {
			std::unique_ptr<Expr> temp = expr();
			op = p.get_binary_operation();
			if (op != RIGHT_BRACKET) {
				throw std::logic_error("Invalid syntax");
			}
			return temp;
		}
	} catch (std::invalid_argument &e) {
	}

	std::unique_ptr<Expr> head;
	try {
		UnaryOperator op = p.get_unary_operation();
		if (op != Factorial) {
			throw std::invalid_argument("Invalid unary operator");
		}
		head = std::make_unique<EUnaryOp>(
			std::move(prod()),
			op
		);
	} catch (std::invalid_argument) {
	}

	try {
		head = std::make_unique<EConst>(p.get_const());
	} catch (std::invalid_argument) {
	}

	try {
		head = std::make_unique<EVariable>(p.get_variable());
	} catch (std::invalid_argument) {
	}

	if (!head)
		throw std::logic_error("Invalid syntax");


	try {
		BinaryOperator op = p.get_binary_operation();

		if (op == MULTIPLICATION) {
			std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
				std::move(head),
				std::move(prod()),
				op);
			return std::move(new_root);
		}

	} catch (std::invalid_argument &e) {
	}
	return std::move(head);
}

std::unique_ptr<Expr> AstBuilder::expr() {
	std::unique_ptr<Expr> left = prod();

	try {
		BinaryOperator op = p.get_binary_operation();
		std::unique_ptr<EBinOp> new_root;

		if (op == PLUS || op == MINUS) {
			new_root = std::make_unique<EBinOp>(
				std::move(left),
				std::move(expr()),
				op);
			return std::move(new_root);
		}
	} catch (std::invalid_argument &e) {
	}
	return std::move(left);
}


std::unique_ptr<Stmt> AstBuilder::stmt() {
	std::cout << "Parsing statement\n";
	p.print();
    try {
        std::unique_ptr<EVariable> ident = std::make_unique<EVariable>(p.get_variable());
        BinaryOperator op;
        try {
            op = p.get_binary_operation();
        } catch (std::invalid_argument) {
        }
        if (op != ASSIGMENT) {
            throw std::logic_error("Invalid syntax: missing assigment");
        }
        std::unique_ptr<Expr> right = expr();
        std::unique_ptr<EAssign> new_root = std::make_unique<EAssign>(
            std::move(ident),
            std::move(right));
        return std::move(new_root);
    } catch (std::invalid_argument) {
    }

    try {
        std::string external_key_word = p.get_keyword();
        if (external_key_word == "while") {
            std::unique_ptr<Expr> result = expr();
            std::string internal_key_word;

            try {
                internal_key_word = p.get_keyword();
            } catch (std::invalid_argument) {
            }
            if (internal_key_word != "do") {
                throw std::logic_error("Invalid syntax: missing do keyword");
            }
            std::unique_ptr<Stmt> while_body_stmts = stmts();

            try {
                internal_key_word = p.get_keyword();
            } catch (std::invalid_argument) {
            }
            if (internal_key_word != "done") {
                throw std::logic_error("Invalid syntax: missing done keyword");
            }

            std::unique_ptr<EWhile> new_root = std::make_unique<EWhile>(
                std::move(result),
                std::move(while_body_stmts));
            return std::move(new_root);
        } else if (external_key_word == "if") {
            std::unique_ptr<Expr> result = expr();
            std::string internal_key_word;

            try {
                internal_key_word = p.get_keyword();
            } catch (std::invalid_argument) {
            }
            if (internal_key_word != "then") {
                throw std::logic_error("Invalid syntax: missing then keyword");
            }

            std::unique_ptr<Stmt> if_body_stmts = stmts();
            std::unique_ptr<Stmt> else_body_stmts;

            try {
                internal_key_word = p.get_keyword();
            } catch (std::invalid_argument) {
            }
            if (internal_key_word == "else") {
                else_body_stmts = stmts();
            }

            try {
                internal_key_word = p.get_keyword();
            } catch (std::invalid_argument) {
            }
            if (internal_key_word != "fi") {
                throw std::logic_error("Invalid syntax: missing fi keyword");
            }

            std::unique_ptr<EIf> new_root = std::make_unique<EIf>(
                std::move(result),
                std::move(if_body_stmts),
                std::move(else_body_stmts));
            return std::move(new_root);
        }
    } catch (std::invalid_argument) {
    }

    throw std::logic_error("Invalid syntax");
}

std::unique_ptr<Stmts> AstBuilder::stmts() {
    std::unique_ptr<Stmts> stmts = std::make_unique<Stmts>();
    while (true) {
        try {
            std::unique_ptr<Stmt> current_stmt = stmt();
            stmts->add_stmt(std::move(current_stmt));
        } catch (std::logic_error) {
            continue;
        }
		if (p.eof()){break;}
    }
    return stmts;
}

}
