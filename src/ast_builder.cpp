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
        const std::size_t init_pos1 = p.get_pos();
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
        p.set_pos(init_pos1);

        std::unique_ptr<Expr> head;
        try {
            head = std::make_unique<EConst>(p.get_const());
        } catch (std::invalid_argument) {
        }

        try {
            head = std::make_unique<EVariable>(p.get_variable());
        } catch (std::invalid_argument) {
        }

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

        } catch (std::invalid_argument &e) {
        }
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

        std::unique_ptr<Expr> result = prod();

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
        } catch (std::invalid_argument &e) {
        }
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
            } catch (std::invalid_argument) {
            }
            if (op != ASSIGMENT) {
                throw std::logic_error("Invalid syntax: missing assigment");
            }
            std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
                std::move(ident),
                std::move(expr()),
                op);
            return std::move(new_root);
        } catch (std::invalid_argument) {
        }
        p.set_pos(init_pos);

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

                std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
                    std::move(result),
                    std::move(while_body_stmts),
                    WHILE);
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

                try {
                    internal_key_word = p.get_keyword();
                } catch (std::invalid_argument) {
                }

                if (internal_key_word == "else") {
                    std::unique_ptr<Stmt> else_body_stmts = stmts();
                    std::unique_ptr<EBinOp> else_root = std::make_unique<EBinOp>(nullptr, std::move(else_body_stmts), ELSE);
                    if_body_stmts->next = std::make_unique<Stmt>(std::move(else_root));
                    try {
                        internal_key_word = p.get_keyword();
                    } catch (std::invalid_argument) {
                    }
                }

                if (internal_key_word != "fi") {
                    throw std::logic_error("Invalid syntax: missing fi keyword");
                }

                std::unique_ptr<EBinOp> new_root = std::make_unique<EBinOp>(
                    std::move(result),
                    std::move(if_body_stmts),
                    IF);
                return std::move(new_root);
            }
        } catch (std::invalid_argument) {
        }
        p.set_pos(init_pos);

        throw std::logic_error("Invalid syntax: missing statement");
    }

    std::unique_ptr<Stmt> AstBuilder::stmts() {
        std::unique_ptr<Stmt> stmt_list;
        try {
            stmt_list = std::make_unique<Stmt>(
                std::move(stmt()),
                std::move(stmts()));
        } catch (std::invalid_argument) {
        }
        return stmt_list;
    }

}
