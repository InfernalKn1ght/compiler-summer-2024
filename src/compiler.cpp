#include "compiler.h"
#include <memory>
#include <string>

namespace AST {
	void Compiler::ast_gen_start() {
		writer.writeLine(".global _start\n");
		writer.writeLine("_start:\n");
    }

    void Compiler::ast_gen_end() { // Temporary function. Later replace with converting integer to string
	    writer.writeLine("\tmv a0, s1\n");
        writer.writeLine("\tli a7, 93\n");
        writer.writeLine("\tecall\n");
    }

    void Compiler::compile(){
		ast_gen_start();
		auto command = builder.stmts();
		command->print();
		writer.writeLine(command->compile());
		ast_gen_end();
	}
}
