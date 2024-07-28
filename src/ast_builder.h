#include "parser.h"
#include "types.h"
#include <memory>
#include <set>

namespace AST {

class AstBuilder {
	private:
		Parser p;
		static std::set<std::string> aviable_registers;		
		std::set<std::shared_ptr<EVariable>> variables;

		void set_register(std::shared_ptr<EVariable>);
		std::shared_ptr<EVariable> get_variable_by_name(std::string);
		std::shared_ptr<Expr> prod();
		std::shared_ptr<Expr> brackets();
		std::unique_ptr<Stmt> stmt();
		std::shared_ptr<Expr> expr();
		std::shared_ptr<Expr> sum();
		std::shared_ptr<Expr> comp();
		std::shared_ptr<Expr> logic_or();
		std::shared_ptr<Expr> logic_and();


	public:
		AstBuilder(std::string str): p(str) {};
		std::unique_ptr<Stmts> stmts();

};

}
