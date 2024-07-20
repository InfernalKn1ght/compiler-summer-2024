#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace AST {

    class Expr {
    public:
        virtual ~Expr() = default;
        virtual void print(const std::string& prefix, bool isLeft) const = 0;
    };

    class EConst : public Expr {
    public:
        EConst(int val) : val(val) {};
        EConst(const std::string &str) : val(std::stoi(str)) {};
        int val;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    class EVariable : public Expr {
    public:
        EVariable(std::string name, std::shared_ptr<EConst> value) :
            value(value), name(name) {};
        EVariable(std::string name) : value(nullptr), name(name) {};
        std::shared_ptr<EConst> value;
        std::string name;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    enum BinaryOperator { PLUS = '+',
                          MINUS = '-',
                          MULTIPLICATION = '*',
                          ASSIGMENT = '=',
                          LEFT_BRACKET = '(',
                          RIGHT_BRACKET = ')'};
    enum UnaryOperator { Factorial = '!' };

    class EOperator : public Expr {};

    class EBinOp : public EOperator {
    public:
        EBinOp(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, BinaryOperator op) :
            left(std::move(left)), right(std::move(right)), op(op) {};
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;
        BinaryOperator op;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    class EUnaryOp : public EOperator {
    public:
        EUnaryOp(std::unique_ptr<Expr> expression, UnaryOperator op) :
            expression(std::move(expression)), op(op) {};
        std::unique_ptr<Expr> expression;
        UnaryOperator op;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    class Stmt {
    public:
        virtual ~Stmt() = default;
        virtual void print(const std::string& prefix="", bool isLeft=false) const = 0;
    };

    class EAssign : public Stmt {
    public:
        EAssign(std::unique_ptr<EVariable> variable, std::unique_ptr<Expr> expr) :
            variable(std::move(variable)), expr(std::move(expr)) {};
        std::unique_ptr<EVariable> variable;
        std::unique_ptr<Expr> expr;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    class EWhile : public Stmt {
    public:
        EWhile(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) :
            condition(std::move(condition)), body(std::move(body)) {};
        std::unique_ptr<Expr> condition;
        std::unique_ptr<Stmt> body;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    class EIf : public Stmt {
    public:
        EIf(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_body, std::unique_ptr<Stmt> else_body) :
            condition(std::move(condition)), then_body(std::move(then_body)), else_body(std::move(else_body)) {};
        std::unique_ptr<Expr> condition;
        std::unique_ptr<Stmt> then_body;
        std::unique_ptr<Stmt> else_body;
        void print(const std::string& prefix="", bool isLeft=false) const override;
    };

    class Stmts : public Stmt {
    public:
        void add_stmt(std::unique_ptr<Stmt> stmt) {
            stmts.push_back(std::move(stmt));
        }
        void print(const std::string& prefix="", bool isLeft=false) const override;
    public:
        std::vector<std::unique_ptr<Stmt>> stmts;
    };
}
