#ifndef PRIMARY_EXP_H
#define PRIMARY_EXP_H
#include "ast.h"

class PrimaryExpCase0
{
public:
    std::unique_ptr<BaseExpAST> exp;
};

class PrimaryExpCase1
{
public:
    std::unique_ptr<BaseAST> l_val;
};

class PrimaryExpCase2
{
public:
    std::unique_ptr<BaseExpAST> number;
};

class PrimaryExpAST : public BaseExpAST
{
public:
    union
    {
        PrimaryExpCase0 *primary_exp_case_0;
        PrimaryExpCase1 *primary_exp_case_1;
        PrimaryExpCase2 *primary_exp_case_2;
    } primary_exp_union;
    void Dump(int ctl = 0) override;
    PrimaryExpAST(int __tag__);
};

#endif