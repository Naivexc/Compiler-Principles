#ifndef MUL_EXP_H
#define MUL_EXP_H

#include "ast.h"

class MulExpCase0
{
public:
    std::unique_ptr<BaseExpAST> unary_exp;
};

class MulExpCase1
{
public:
    std::unique_ptr<BaseExpAST> mul_exp;
    std::unique_ptr<BaseOpAST> mul_op;
    std::unique_ptr<BaseExpAST> unary_exp;
};

class MulExpAST : public BaseExpAST
{
public:
    union
    {
        MulExpCase0 *mul_exp_case_0;
        MulExpCase1 *mul_exp_case_1;
    } mul_exp_union;
    void Dump(int ctl = 0) override;
    MulExpAST(int __tag__);
};

#endif