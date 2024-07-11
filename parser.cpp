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

int econst() {
    std::string acc;
    while (pos < text.size() && isdigit(text[pos])) {
        acc.push_back(text[pos]);
        ++pos;
    }

    if (acc.size() > 0) {
        return std::stoi(acc);
    } else {
        return -1;
    }
}

std::string ident_or_keyword() {
    std::string acc;
    while (pos < text.size() && isalpha(text[pos])) {
        acc.push_back(text[pos]);
        ++pos;
    }
    if (acc.size() > 0) {
        return acc;
    } else {
        return "-1";
    }
}

std::string ident() {
    std::string s = ident_or_keyword();
    if (is_keyword(s)) {
        return "-1";
    } else {
        return s;
    }
}
