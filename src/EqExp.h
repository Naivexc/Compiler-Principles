#ifndef EQ_EXP_H
#define EQ_EXP_H

#include "ast.h"

class EqExpCase0
{
public:
    std::unique_ptr<BaseExpAST> rel_exp;
};

class EqExpCase1
{
public:
    std::unique_ptr<BaseExpAST> eq_exp;
    std::unique_ptr<BaseOpAST> eq_op;
    std::unique_ptr<BaseExpAST> rel_exp;
};

#endif