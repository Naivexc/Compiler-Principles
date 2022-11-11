#ifndef ADD_EXP_H
#define ADD_EXP_H

#include "Base.h"
#include <memory>

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

class AddExpAST : public BaseExpAST
{
public:
    union
    {
        AddExpCase0 *add_exp_case_0;
        AddExpCase1 *add_exp_case_1;
    } add_exp_union;
    void Dump(int ctl = 0) override;
    AddExpAST(int __tag__);
};

#endif