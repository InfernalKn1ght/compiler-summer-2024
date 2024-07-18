#pragma once

#include "IO.h"
#include "types.h"
#include <memory>
#include <string>

namespace AST{

class Compiler{
private:
	std::string current_line;
	Reader reader;
	Writer writer;

	void ast_gen_start();
	void ast_gen_end();
	void ast_gen_expr(std::unique_ptr<Expr> root);

public:
	Compiler(std::string input_file, std::string output_file): reader(input_file), writer(output_file){};
	void compile(); 
};

}
