#pragma once

class Expr {
};

class EConst : public Expr {
    int val;
};

enum Operator { ADD,
                SUB,
                MUL };

class EBinop : public Expr {
    Expr *left;
    Expr *right;
    Operator op;
};