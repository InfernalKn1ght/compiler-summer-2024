#include "parser.h"
#include "ast.h"

std::string text;
int pos;

bool is_whitespace(char c) {
    switch (c) {
    case ' ':
        return 1;
        break;
    case '\n':
        return 1;
        break;
    case '\t':
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

void ws() {
    while (pos < text.size() && is_whitespace(text[pos])) {
        ++pos;
    }
}

