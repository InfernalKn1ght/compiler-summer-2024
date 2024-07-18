#include <iostream>
#include "compiler.h"

int main() {
	AST::Compiler compiler("input.txt", "output.txt");
	compiler.compile();
    return EXIT_SUCCESS;
}
