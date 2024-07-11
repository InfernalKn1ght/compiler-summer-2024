#pragma once
#include <string>

extern std::string text;
extern int pos;

bool is_whitespace(char c);
void ws();
int econst();
bool is_keyword(std::string s);
std::string ident_or_keyword();
std::string ident();