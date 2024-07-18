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
		void stmts();
		std::unique_ptr<Expr> stmt();
		std::unique_ptr<Expr> expr();

};

}
