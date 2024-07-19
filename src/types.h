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
        EConst(const std::string &str) : val(std::stoi(str)) {};
        int val;
    };

    class EVariable : public Expr {
    public:
        EVariable(std::string name, std::shared_ptr<EConst> value) : value(value), name(name) {};
        EVariable(std::string name) : value(nullptr), name(name) {};
        std::shared_ptr<EConst> value;
        std::string name;
    };

    enum BinaryOperator { PLUS = '+',
                          MINUS = '-',
                          MULTIPLICATION = '*',
                          ASSIGMENT = '=',
                          LEFT_BRACKET = '(',
                          RIGHT_BRACKET = ')',
						  WHILE,
                          IF,
                          ELSE };
    enum UnaryOperator { Factorial = '!' };

    class EOperator : public Expr {};

    class EBinOp : public EOperator {
    public:
        EBinOp(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, BinaryOperator op) : left(std::move(left)), right(std::move(right)), op(op) {};
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;
        BinaryOperator op;
    };

    class EUnaryOp : public EOperator {
    public:
        EUnaryOp(std::unique_ptr<Expr> expression, UnaryOperator op) : expression(std::move(expression)), op(op) {};
        std::unique_ptr<Expr> expression;
        UnaryOperator op;
    };

    class Stmt: public Expr {
        public:
        std::unique_ptr<EBinOp> tree;
        std::unique_ptr<Stmt> next;
        Stmt(std::unique_ptr<EBinOp> tree, std::unique_ptr<Stmt> next = nullptr) : tree(std::move(tree)), next(std::move(next)) {}
    };

}
