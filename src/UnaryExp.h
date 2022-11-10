#ifndef UNARY_EXP_H
#define UNARY_EXP_H
#include "ast.h"

struct UnaryExpCase0
{
    std::unique_ptr<BaseExpAST> primary_exp;
};

struct UnaryExpCase1
{
    std::string ident;
    std::unique_ptr<BaseAST> func_r_params;
};

struct UnaryExpCase2
{
    std::unique_ptr<BaseOpAST> unary_op;
    std::unique_ptr<BaseExpAST> unary_exp;
};
#endif