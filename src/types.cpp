#include "types.h"

#include <iostream>
#include <string>

namespace AST {

    void EConst::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << val << "\n";
    }

	std::string EConst::compile() const{
		return "	li a0, " + std::to_string(val); 
	}

    void EVariable::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << name + "(" + register_name + ")" << "\n";
    }

	std::string EVariable::compile() const{
		return "	li a0, " + register_name; 
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
		result.append("    addi sp, sp, -8");
		result.append("    sd a0, (sp)");
		result.append(right->compile());
		result.append("    ld a1, (sp)");

		switch (op){
			case PLUS:
				result.append("    add a0, a0, a1");
				break;
			case MINUS:
				result.append("    sub a0, a0, a1");
				break;
			case MULTIPLICATION:
				result.append("    mul a0, a0, a1");
				break;
		}

		result.append("    addi sp, sp, 8");
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
		//TODO
		return "";
	}

    void EWhile::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "While" << "\n";
        condition->print(prefix + (isLeft ? "│   " : "    "), true);
        body->print(prefix + (isLeft ? "│   " : "    "), false);
    }

	std::string EWhile::compile() const {
		//TODO
		return "";
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

	std::string EIf::compile() const {
		//TODO
		return "";
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
