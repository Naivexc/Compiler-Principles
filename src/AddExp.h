#ifndef ADD_EXP_H
#define ADD_EXP_H

#include "ast.h"

class AddExpCase0
{
public:
    std::unique_ptr<BaseExpAST> mul_exp;
};

class AddExpCase1
{
public:
    std::unique_ptr<BaseExpAST> mul_exp;
    std::unique_ptr<BaseExpAST> add_exp;
    std::unique_ptr<BaseOpAST> add_op;
};

#endif