#include "compiler.h"
#include <string>
#include "ast_builder.h"

namespace AST {
    void Compiler::ast_gen_start() {
		writer.writeLine(".global _start");
        writer.writeLine("_start:");
    }

    void Compiler::ast_gen_end() { // Temporary function. Later replace with converting integer to string
        writer.writeLine("	li a7, 93");
        writer.writeLine("	ecall");
    }

    void Compiler::ast_gen_expr(std::unique_ptr<Expr> root) {
        if (AST::EConst *econ = dynamic_cast<AST::EConst *>(root.get())) {
            writer.writeLine("	li a0, " + std::to_string(econ->val));
        } else if (AST::EBinOp *ebinop = dynamic_cast<AST::EBinOp *>(root.get())) {
            ast_gen_expr(std::move(ebinop->left));

            writer.writeLine("	addi sp, sp, -8");
            writer.writeLine("	sd a0, (sp)");

            ast_gen_expr(std::move(ebinop->right));
            writer.writeLine("	ld a1, (sp)");

            switch (ebinop->op) {
            case PLUS:
                writer.writeLine("	add a0, a0, a1");
                break;
            case MULTIPLICATION:
                writer.writeLine("	mul a0, a0, a1");
                break;
            default:
                break;
            }
            writer.writeLine("	addi sp, sp, 8");
        }
    }

	void Compiler::compile(){
		ast_gen_start();
		while (reader.readLine(current_line)) {
			AstBuilder builder = current_line;
			auto command = builder.expr();
			ast_gen_expr(std::move(command));
		}
		ast_gen_end();
	}
}
