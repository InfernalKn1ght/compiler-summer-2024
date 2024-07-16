#include "parser.h"
#include "types.h"
#include <memory>

namespace AST {

class AST {
	private:
		Parser p;

	public:
		AST(std::string);
		std::unique_ptr<Expr> get_root();

};

}
