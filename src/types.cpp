#include "types.h"

#include <iostream>

namespace AST {

    void EConst::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << val << "\n";
    }

    void EVariable::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << name << "\n";
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
                case ASSIGMENT:
                    return ":=";
                default:
                    return "";
            }
        };
        std::cout << prefix << (isLeft ? "├──" : "└──") << op_str() << "\n";
        left->print(prefix + (isLeft ? "│   " : "    "), true);
        right->print(prefix + (isLeft ? "│   " : "    "), false);
    }

    void EUnaryOp::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "!"
                  << "\n";
        expression->print(prefix + (isLeft ? "│   " : "    "), true);
    }

    void EAssign::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << ":=" << "\n";
        variable->print(prefix + (isLeft ? "│   " : "    "), true);
        expr->print(prefix + (isLeft ? "│   " : "    "), false);
    }

    void EWhile::print(const std::string& prefix, bool isLeft) const {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "While" << "\n";
        condition->print(prefix + (isLeft ? "│   " : "    "), true);
        body->print(prefix + (isLeft ? "│   " : "    "), false);
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

    void Stmts::print(const std::string& prefix, bool isLeft) const {
        for (const auto& stmt : stmts) {
            stmt->print(prefix, isLeft);
        }
    }
}
