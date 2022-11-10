#ifndef CONST_DEF_H
#define CONST_DEF_H

#include "ast.h"

class ConstDefCase0
{
public:
    std::string ident;
    std::unique_ptr<BaseAST> const_init_val;
    CalData data;
};

class ConstDefCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_const_exp;
    std::unique_ptr<BaseAST> const_init_val;
    CalData data;
    std::string return_type();
};

#endif