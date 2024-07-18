#include "types.h"
#include "memory"
#include <iostream>

namespace AST {
    void ast_gen_start() {
        std::cout << ".global _start" << std::endl;
        std::cout << "_start:" << std::endl;
    }

    void ast_gen_end() { // Temporary function. Later replace with converting integer to string
        std::cout << "  li a7, 93" << std::endl;
        std::cout << "  " << std::endl;
    }

    void ast_gen_expr(std::unique_ptr<Expr> root) {
        if (AST::EConst *econ = dynamic_cast<AST::EConst *>(root.get())) {
            std::cout << "      li a0, " << econ->val << std::endl;
        } else if (AST::EBinOp *ebinop = dynamic_cast<AST::EBinOp *>(root.get())) {
            ast_gen_expr(std::move(ebinop->left));

            std::cout << "      addi sp, sp, -8" << std::endl;
            std::cout << "      sd a0, (sp)" << std::endl;

            ast_gen_expr(std::move(ebinop->right));
            std::cout << "      ld a1, (sp)" << std::endl;

            switch (ebinop->op) {
            case PLUS:
                std::cout << "  add a0, a0, a1" << std::endl;
                break;
            case MULTIPLICATION:
                std::cout << "  mul a0, a0, a1" << std::endl;
                break;
            default:
                break;
            }
            std::cout << "      addi sp, sp, 8" << std::endl;
        }
    }

    const std::string EBinOp::record() const {
        return " ";
    }

    const std::string EUnaryOp::record() const {
        // TODO
        return " ";
    }
}
