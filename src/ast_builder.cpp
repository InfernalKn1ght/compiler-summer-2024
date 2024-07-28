#include "ast_builder.h"
#include "parser.h"
#include "types.h"

#include <cstddef>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

namespace AST {
std::set<std::string> AstBuilder::aviable_registers = {
	"s1",
	"s2",
	"s3",
	"s4",
	"s5",
	"s6",
	"s7",
	"s8",
	"s9",
	"s10",
	"s11"
};

std::shared_ptr<Expr> AstBuilder::brackets(){
	auto before_brackets_pos = p.get_pos();
	try {
		Bracket op = p.get_bracket();

		if (op == LEFT_BRACKET) {
			std::shared_ptr<Expr> head = expr();
			op = p.get_bracket();
			if (op != RIGHT_BRACKET) {
				throw std::logic_error("Invalid syntax: missing right bracket");
			}
			return std::move(head);
		}

	} catch (std::invalid_argument &e) {
	}
	p.set_pos(before_brackets_pos);
	throw std::invalid_argument("Missing brackets");
}

std::shared_ptr<Expr> AstBuilder::prod() {
	std::shared_ptr<Expr> head;

	try {
		head = std::make_unique<EConst>(p.get_const());
	} catch (std::invalid_argument) {
		try{
			head = get_variable_by_name(p.get_variable());
		} catch (std::invalid_argument) {
			head = brackets();
		}
	}

	if (!head)
		throw std::logic_error("Invalid syntax: missing head of expression");

	try {
		UnaryOperator op = p.get_unary_operation();
		if (op != Factorial) {
			throw std::invalid_argument("Invalid unary operator");
		}
		head = std::make_shared<EUnaryOp>(
			std::move(head),
			op
		);
	} catch (std::invalid_argument) {
	}

	auto after_head_pos = p.get_pos();
	try {
		BinaryOperator op = p.get_binary_operation();

		if (op == MULTIPLICATION) {
			head = std::make_shared<EBinOp>(
				std::move(head),
				std::move(prod()),
				op);
		}else{
			p.set_pos(after_head_pos);
		}
	} catch (std::invalid_argument &e) {
		p.set_pos(after_head_pos);
	}
	return std::move(head);
}

std::shared_ptr<Expr> AstBuilder::sum() {
	std::shared_ptr<Expr> left = prod();

	auto after_left_pos = p.get_pos();
	try {
		BinaryOperator op = p.get_binary_operation();
		std::shared_ptr<EBinOp> new_root;

		if (op == PLUS || op == MINUS) {
			new_root = std::make_shared<EBinOp>(
				std::move(left),
				std::move(sum()),
				op);
			return std::move(new_root);
		}
	} catch (std::invalid_argument &e) {
	}
	p.set_pos(after_left_pos);
	return std::move(left);
}

std::shared_ptr<Expr> AstBuilder::comp() {
	std::shared_ptr<Expr> left = sum();

	auto after_left_pos = p.get_pos();
	try {
		BinaryOperator op = p.get_binary_operation();
		std::shared_ptr<EBinOp> new_root;

		if (op == LESS || op == GREATER || op == EQUAL) {
			new_root = std::make_shared<EBinOp>(
				std::move(left),
				std::move(comp()),
				op);
			return std::move(new_root);
		}
	} catch (std::invalid_argument &e) {
	}
	p.set_pos(after_left_pos);
	return std::move(left);
}

std::shared_ptr<Expr> AstBuilder::logic_and() {
	std::shared_ptr<Expr> left = comp();

	auto after_left_pos = p.get_pos();
	try {
		BinaryOperator op = p.get_binary_operation();
		std::shared_ptr<EBinOp> new_root;

		if (op == AND) {
			new_root = std::make_shared<EBinOp>(
				std::move(left),
				std::move(logic_and()),
				op);
			return std::move(new_root);
		}
	} catch (std::invalid_argument &e) {
	}
	p.set_pos(after_left_pos);
	return std::move(left);
}

std::shared_ptr<Expr> AstBuilder::logic_or() {
	std::shared_ptr<Expr> left = logic_and();

	auto after_left_pos = p.get_pos();
	try {
		BinaryOperator op = p.get_binary_operation();
		std::shared_ptr<EBinOp> new_root;

		if (op == OR) {
			new_root = std::make_shared<EBinOp>(
				std::move(left),
				std::move(logic_or()),
				op);
			return std::move(new_root);
		}
	} catch (std::invalid_argument &e) {
	}
	p.set_pos(after_left_pos);
	return std::move(left);
}

std::shared_ptr<Expr> AstBuilder::expr() {
	return std::move(logic_or());
}

std::unique_ptr<Stmt> AstBuilder::stmt() {
	auto init_pos = p.get_pos();

    try {
		std::shared_ptr<EVariable> variable = get_variable_by_name(p.get_variable());
        BinaryOperator op;
        try {
            op = p.get_binary_operation();
        } catch (std::invalid_argument) {
        }
        if (op != ASSIGMENT) {
            throw std::logic_error("Invalid syntax: missing assigment");
        }
        std::shared_ptr<Expr> right = expr();
        std::unique_ptr<EAssign> new_root = std::make_unique<EAssign>(
            variable,
            right);
        return std::move(new_root);
    } catch (std::invalid_argument) {
    }
	p.set_pos(init_pos);

    try {
        std::string external_key_word = p.get_keyword();
        if (external_key_word == "while") {
            std::shared_ptr<Expr> condition = expr();

			std::string internal_key_word = p.get_keyword();
            if (internal_key_word != "do") {
                throw std::logic_error("Invalid syntax: missing do keyword");
            }
			std::unique_ptr<Stmts> while_body_stmts;
			try{
				while_body_stmts = stmts();
			} catch (std::invalid_argument){}

			std::string external_key_word = p.get_keyword();
            if (external_key_word != "done") {
                throw std::logic_error("Invalid syntax: missing done keyword");
            }

            std::unique_ptr<EWhile> new_root = std::make_unique<EWhile>(
                std::move(condition),
                std::move(while_body_stmts));
            return std::move(new_root);
        } else if (external_key_word == "if") {
            std::shared_ptr<Expr> condition = expr();

            std::string internal_key_word = p.get_keyword();
            if (internal_key_word != "then") {
                throw std::logic_error("Invalid syntax: missing then keyword");
            }
			
            std::unique_ptr<Stmts> if_body_stmts;
			try {
				if_body_stmts = stmts();
			} catch (std::invalid_argument) {
			}
            std::unique_ptr<Stmts> else_body_stmts = std::make_unique<Stmts>();

			std::string external_key_word = p.get_keyword();
            if (external_key_word == "else") {
                else_body_stmts = stmts();
				external_key_word = p.get_keyword();
            }

            if (external_key_word != "fi") {
                throw std::logic_error("Invalid syntax: missing fi keyword");
            }
            std::unique_ptr<EIf> new_root = std::make_unique<EIf>(
                std::move(condition),
                std::move(if_body_stmts),
                std::move(else_body_stmts));
            return std::move(new_root);
        }
    } catch (std::invalid_argument) {
    }
	p.set_pos(init_pos);
    throw std::invalid_argument("Invalid syntax: missing statement");
}

std::unique_ptr<Stmts> AstBuilder::stmts() {
    std::unique_ptr<Stmts> stmts = std::make_unique<Stmts>();
    while (!p.eof()) {
		std::size_t init_pos = p.get_pos();
        try {
            std::unique_ptr<Stmt> current_stmt = stmt();
            stmts->add_stmt(std::move(current_stmt));
        } catch (std::invalid_argument) {
			p.set_pos(init_pos);
            break;
        }
    }
    return stmts;
}

void AstBuilder::set_register(std::shared_ptr<EVariable> variable){
	if (aviable_registers.size() == 0) throw std::logic_error("It is not possible to allocate a register for variable " + variable->name);
	std::string poped_aviable_name = aviable_registers.extract(aviable_registers.begin()).value();
	variable->register_name = poped_aviable_name;
}

std::shared_ptr<EVariable> AstBuilder::get_variable_by_name(std::string name){
	for (auto& cur_var : variables){
		if (cur_var->name == name) return cur_var;
	}

	std::shared_ptr<EVariable> new_variable = std::make_shared<EVariable>(name);
	set_register(new_variable);
	variables.insert(new_variable);
	return new_variable;
}

}
