#include "parser.h"
#include "types.h"
#include <memory>

namespace AST {

class AstBuilder {
	private:
		Parser p;

		std::unique_ptr<Expr> prod();


	public:
		AstBuilder(std::string str): p(str) {};
		std::unique_ptr<Stmts> stmts();
		std::unique_ptr<Stmt> stmt();
		std::unique_ptr<Expr> expr();

};

}
