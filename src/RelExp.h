#ifndef REL_EXP_H
#define REL_EXP_H

#include "ast.h"

class RelExpCase0
{
public:
    std::unique_ptr<BaseExpAST> add_exp;
};

class RelExpCase1
{
public:
    std::unique_ptr<BaseExpAST> rel_exp;
    std::unique_ptr<BaseOpAST> rel_op;
    std::unique_ptr<BaseExpAST> add_exp;
};

#endif