#include "types.h"

#include <iostream>
#include <string>

namespace AST {

    void EConst::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << val << "\n";
    }

	std::string EConst::compile() const{
		return "\tli t0, " + std::to_string(val) + "\n"; 
	}

    void EVariable::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << name << "\n";
    }

	std::string EVariable::compile() const{
		return "\tmv t0, " + register_name + "\n"; 
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
                case LESS:
                    return "<";
                case GREATER:
                    return ">";
                case EQUAL:
                    return "=";
                case AND:
                    return "and";
                case OR:
                    return "or";
                default:
                    return "";
            }
        };
        std::cout << prefix << (isLeft ? "├──" : "└──") << op_str() << "\n";
        left->print(prefix + (isLeft ? "│   " : "    "), true);
        right->print(prefix + (isLeft ? "│   " : "    "), false);
    }

    unsigned EBinOp::nesting_level = 1;
	std::string EBinOp::compile() const {
		std::string result;

        std::string nesting_level_str = std::to_string(nesting_level);
		nesting_level++;

		result.append(left->compile());
		result.append("\taddi sp, sp, -8\n");
		result.append("\tsd t0, (sp)\n");
		result.append(right->compile());
		result.append("\tld t1, (sp)\n");

		switch (op){
			case PLUS:
				result.append("\tadd t0, t0, t1\n");
				break;
			case MINUS:
				result.append("\tsub t0, t1, t0\n");
				break;
			case MULTIPLICATION:
				result.append("\tmul t0, t0, t1\n");
				break;
            case LESS:
				result.append("\tslt t0, t1, t0\n");
				break;
            case GREATER:
				result.append("\tslt t0, t0, t1\n");
				break;
            case EQUAL:
				result.append("\txor t0, t1, t0\n");
                result.append("\tbne t0, zero, eq_st" + nesting_level_str + "\n");
                result.append("\tli t0, 1\n");
                result.append("\tj eq_end" + nesting_level_str + "\n");
                result.append("eq_st" + nesting_level_str + ":\n");
                result.append("\tmv t0, zero\n");
                result.append("eq_end" + nesting_level_str + ":\n");
				break;
            case AND:
                result.append("\tbeq t0, zero, and_st" + nesting_level_str + "\n");
                result.append("\tbeq t1, zero, and_st" + nesting_level_str + "\n");
                result.append("\tli t0, 1\n");
                result.append("\tj and_end" + nesting_level_str + "\n");
                result.append("and_st" + nesting_level_str + ":\n");
                result.append("\tmv t0, zero\n");
                result.append("and_end" + nesting_level_str + ":\n");
				break;
            case OR:
				result.append("\tor t0, t0, t1\n");
                result.append("\tbeq t0, zero, or_st" + nesting_level_str + "\n");
                result.append("\tli t0, 1\n");
                result.append("\tj or_end" + nesting_level_str + "\n");
                result.append("or_st" + nesting_level_str + ":\n");
                result.append("\tmv t0, zero\n");
                result.append("or_end" + nesting_level_str + ":\n");
				break;
		}

		result.append("\taddi sp, sp, 8\n");
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
        result.append("\tbeq t0, zero, while_end" + nesting_level_str + "\n");
        result.append(body->compile());
        result.append("\tj while_st" + nesting_level_str + "\n");
        result.append("while_end" + nesting_level_str + ":\n");
        
		return result;
	}

    void EIf::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "If" << "\n";
        condition->print(prefix + (isLeft ? "│   " : "    "), true);
        then_body->print(prefix + (isLeft ? "│   " : "    "), true);
        if (else_body->size() != 0) {
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
        result.append("\tbeq t0, zero, else_st" + nesting_level_str + "\n");
        result.append(then_body->compile());
        result.append("\tj else_end" + nesting_level_str + "\n");
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

	std::size_t Stmts::size() const {
		return stmts.size();
	}
}
