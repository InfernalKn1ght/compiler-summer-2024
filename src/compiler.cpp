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

    void Compiler::compile(){
		ast_gen_start();
		auto command = builder.stmts();
		command->print();
		writer.writeLine(command->compile());
		ast_gen_end();
	}
}
