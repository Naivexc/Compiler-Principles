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

class EqExpAST : public BaseExpAST
{
public:
    union
    {
        EqExpCase0 *eq_exp_case_0;
        EqExpCase1 *eq_exp_case_1;
    } eq_exp_union;
    void Dump(int ctl = 0) override;
    EqExpAST(int __tag__);
};

#endif