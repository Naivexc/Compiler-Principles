#ifndef CONST_INIT_VAL_H
#define CONST_INIT_VAL_H

#include "ast.h"

class ConstInitValCase0
{
public:
    std::unique_ptr<BaseExpAST> const_exp;
    CalData data;
};

class ConstInitValCase1
{
public:
    std::vector<std::unique_ptr<BaseAST>> vec_const_init_val;
};

#endif