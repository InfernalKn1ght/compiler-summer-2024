#pragma once

#include "IO.h"
#include "types.h"
#include "ast_builder.h"
#include <memory>
#include <string>

namespace AST{

class Compiler{
private:
	std::string text_of_file;
	Reader reader;
	Writer writer;
	AstBuilder builder;

	void ast_gen_start();
	void ast_gen_end();
	void ast_gen_expr(std::unique_ptr<Expr> root);
	void ast_gen_stmts(std::unique_ptr<Stmts> stmts);
	void ast_gen_stmt(std::unique_ptr<Stmt> stmt);
	void ast_gen_while(std::unique_ptr<EWhile> st);
	void ast_gen_if(std::unique_ptr<EIf> st);
	void ast_gen_assignment(std::unique_ptr<EAssign> st);

public:
	Compiler(std::string input_file, std::string output_file):
		reader(input_file), writer(output_file), builder(reader.readFile()) {};
	void compile(); 
};

}
