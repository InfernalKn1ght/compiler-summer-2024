#include <iostream>
#include "compiler.h"

int main() {
	AST::Compiler compiler("input.txt", "output.s");
	compiler.compile();
    return EXIT_SUCCESS;
}
