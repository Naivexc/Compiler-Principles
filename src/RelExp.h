#ifndef REL_EXP_H
#define REL_EXP_H

#include "Base.h"
#include <memory>
#include <cassert>

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

class RelExpAST : public BaseExpAST
{
public:
    union
    {
        RelExpCase0 *rel_exp_case_0;
        RelExpCase1 *rel_exp_case_1;
    } rel_exp_union;
    void Dump(int ctl = 0) override;
    RelExpAST(int __tag__);
};

#endif