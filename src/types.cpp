#include "types.h"

#include <iostream>
#include <string>

namespace AST {

    void EConst::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << val << "\n";
    }

	std::string EConst::compile() const{
		return "	li t0, " + std::to_string(val) + "\n"; 
	}

    void EVariable::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << name + "(" + register_name + ")" << "\n";
    }

	std::string EVariable::compile() const{
		return "	mv t0, " + register_name + "\n"; 
	}

    void EBinOp::print(const std::string& prefix, bool isLeft) const {
        auto op_str = [this]() -> std::string{
            switch (op) {
                case PLUS:
                    return "+";
                case MINUS:
                    return "-";
                case MULTIPLICATION:
                    return "*";
                default:
                    return "";
            }
        };
        std::cout << prefix << (isLeft ? "├──" : "└──") << op_str() << "\n";
        left->print(prefix + (isLeft ? "│   " : "    "), true);
        right->print(prefix + (isLeft ? "│   " : "    "), false);
    }

	std::string EBinOp::compile() const {
		std::string result;
		result.append(left->compile());
		result.append("	addi sp, sp, -8\n");
		result.append("	sd t0, (sp)\n");
		result.append(right->compile());
		result.append("	ld t1, (sp)\n");

		switch (op){
			case PLUS:
				result.append("	add t0, t0, t1\n");
				break;
			case MINUS:
				result.append("	sub t0, t1, t0\n");
				break;
			case MULTIPLICATION:
				result.append("	mul t0, t0, t1\n");
				break;
		}

		result.append("	addi sp, sp, 8\n");
		return result;
	}

    void EUnaryOp::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├── " : "└── ") << char(op)
                  << "\n";
        expression->print(prefix + (isLeft ? "│   " : "    "), false);
    }

	std::string EUnaryOp::compile() const {
		//TODO
		return "";
	}

    void EAssign::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << ":=" << "\n";
        variable->print(prefix + (isLeft ? "│   " : "    "), true);
        expr->print(prefix + (isLeft ? "│   " : "    "), false);
    }

	std::string EAssign::compile() const{
		std::string result;
		result.append(expr->compile());
		result.append("	mv " + variable->register_name + ", t0\n");
		return result;
	}

    void EWhile::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "While" << "\n";
        condition->print(prefix + (isLeft ? "│   " : "    "), true);
        body->print(prefix + (isLeft ? "│   " : "    "), false);
    }

	unsigned EWhile::nesting_level = 1;
	std::string EWhile::compile() const {
        std::string result;

		std::string nesting_level_str = std::to_string(nesting_level);
		nesting_level++;
        result.append("while_st" + nesting_level_str + ":\n");
		result.append(condition->compile());
        result.append("	beq t0, zero, while_end" + nesting_level_str + "\n");
        result.append(body->compile());
        result.append("	j while_st" + nesting_level_str + "\n");
        result.append("while_end" + nesting_level_str + ":\n");
        
		return result;
	}

    void EIf::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "If" << "\n";
        condition->print(prefix + (isLeft ? "│   " : "    "), true);
        then_body->print(prefix + (isLeft ? "│   " : "    "), true);
        if (else_body != nullptr) {
            std::cout << prefix << (isLeft ? "├──" : "└──") << "Else" << "\n";
            else_body->print(prefix + (isLeft ? "│   " : "    "), false);
        }
    }

	unsigned EIf::nesting_level = 1;
	std::string EIf::compile() const {
	    std::string result;

		std::string nesting_level_str = std::to_string(nesting_level);
		nesting_level++;
        result.append(condition->compile());
        result.append("	beq t0, zero, else_st" + nesting_level_str + "\n");
        result.append(then_body->compile());
        result.append("	j else_end" + nesting_level_str + "\n");
        result.append("else_st" + nesting_level_str + ":\n");
        result.append(else_body->compile());
        result.append("else_end" + nesting_level_str + ":\n");

		return result;
	}

    void Stmts::print(const std::string& prefix, bool isLeft) const {
        for (const auto& stmt : stmts) {
            stmt->print(prefix, isLeft);
        }
    }

	std::string Stmts::compile() const {
		std::string result = "";
        for (const auto& stmt : stmts) {
			result.append(stmt->compile());
        }
		return result;
	}
}
