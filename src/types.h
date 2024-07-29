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
		virtual std::string compile() const = 0;
    };

    class EConst : public Expr {
    public:
        EConst(int val) : val(val) {};
        EConst(const std::string &str) : val(std::stoi(str)) {};
        int val;
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override; 
    };

    class EVariable : public Expr {
    public:
        EVariable(std::string name) : name(name) {};
		std::string name;
        std::string register_name;
		/* void set_register_name(std::string register_name){this->register_name = register_name;}; */
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override; 
    };

    enum BinaryOperator { PLUS = '+',
                          MINUS = '-',
                          MULTIPLICATION = '*',
                          ASSIGMENT = 128,
                          AND = 129,
                          OR = 130,
                          LESS = '<',
                          GREATER = '>',
                          EQUAL = '='
	};

	enum Bracket {
					LEFT_BRACKET = '(',
					RIGHT_BRACKET = ')'
	};
    enum UnaryOperator { Factorial = '!' };

    class EOperator : public Expr {};

    class EBinOp : public EOperator {
    public:
        EBinOp(std::shared_ptr<Expr> left, std::shared_ptr<Expr> right, BinaryOperator op) :
            left(left), right(right), op(op) {};
        std::shared_ptr<Expr> left;
        std::shared_ptr<Expr> right;
        BinaryOperator op;
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override;
    };

    class EUnaryOp : public EOperator {
    public:
        EUnaryOp(std::shared_ptr<Expr> expression, UnaryOperator op) :
            expression(std::move(expression)), op(op) {};
        std::shared_ptr<Expr> expression;
        UnaryOperator op;
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override;
    };

    class Stmt {
    public:
        virtual ~Stmt() = default;
        virtual void print(const std::string& prefix="", bool isLeft=false) const = 0;
		virtual std::string compile() const = 0;
    };

    class EAssign : public Stmt {
    public:
        EAssign(std::shared_ptr<EVariable> variable, std::shared_ptr<Expr> expr) :
            variable(variable), expr(expr) {};
        std::shared_ptr<EVariable> variable;
		std::shared_ptr<Expr> expr;
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override;
    };

    class Stmts : public Stmt {
    public:
        void add_stmt(std::unique_ptr<Stmt> stmt) {
            stmts.push_back(std::move(stmt));
        }
        void print(const std::string& prefix="", bool isLeft=false) const;
		std::string compile() const;
		std::size_t size() const;
	private:
        std::vector<std::unique_ptr<Stmt>> stmts;
    };

    class EWhile : public Stmt {
    public:
        EWhile(std::shared_ptr<Expr> condition, std::unique_ptr<Stmts> body) :
            condition(condition), body(std::move(body)) {};
		static unsigned nesting_level;
        std::shared_ptr<Expr> condition;
        std::unique_ptr<Stmts> body;
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override;
    };

    class EIf : public Stmt {
    public:
        EIf(std::shared_ptr<Expr> condition, std::unique_ptr<Stmts> then_body, std::unique_ptr<Stmts> else_body) :
            condition(condition), then_body(std::move(then_body)), else_body(std::move(else_body)) {};
		static unsigned nesting_level;
        std::shared_ptr<Expr> condition;
        std::unique_ptr<Stmts> then_body;
        std::unique_ptr<Stmts> else_body;
        void print(const std::string& prefix="", bool isLeft=false) const override;
		std::string compile() const override;
    };

}
