#ifndef L_AND_EXP_H
#define L_AND_EXP_H

#include "ast.h"

class LAndExpCase0
{
public:
    std::unique_ptr<BaseExpAST> eq_exp;
};

class LAndExpCase1
{
public:
    std::unique_ptr<BaseExpAST> l_and_exp;
    std::unique_ptr<BaseOpAST> l_and_op;
    std::unique_ptr<BaseExpAST> eq_exp;
};

class LAndExpAST : public BaseExpAST
{
public:
    union
    {
        LAndExpCase0 *l_and_exp_case_0;
        LAndExpCase1 *l_and_exp_case_1;
    } l_and_exp_union;
    void Dump(int ctl = 0) override;
    LAndExpAST(int __tag__);
};

#endif