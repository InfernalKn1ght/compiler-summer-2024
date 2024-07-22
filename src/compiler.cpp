#include "compiler.h"
#include <memory>
#include <string>

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

    void Compiler::ast_gen_stmts(std::unique_ptr<Stmts> stmts) {
        for (const auto& stmt : stmts->stmts) {
            ast_gen_stmt(std::move(stmt))
        }
    }

    void Compiler::ast_gen_stmt(std::unique_ptr<Stmt> stmt) {
        if (EWhile *ewh = dynamic_cast<EWhile *>(stmt.get())) {
            std::unique_ptr<EWhile> p_ewh(ewh);
            ast_gen_while(std::move(p_ewh));
        } else if (EIf *eif = dynamic_cast<EIf *>(stmt.get())) {
            std::unique_ptr<EIf> p_eif(eif);
            ast_gen_if(std::move(p_eif));
        } else if (EAssign *eassgn = dynamic_cast<EAssign *>(stmt.get())) {
            std::unique_ptr<EAssign> p_eassgn(eassgn);
            ast_gen_assignment(std::move(p_eassgn));
        }
    }

    void Compiler::ast_gen_while(std::unique_ptr<EWhile> s) {

    }

    void Compiler::compile(){
		auto command = builder.stmts();
		command->print();
	}
}
