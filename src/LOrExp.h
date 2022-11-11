#ifndef L_OR_EXP_H
#define L_OR_EXP_H

#include "ast.h"

class LOrExpCase0
{
public:
    std::unique_ptr<BaseExpAST> l_and_exp;
};

class LOrExpCase1
{
public:
    std::unique_ptr<BaseExpAST> l_or_exp;
    std::unique_ptr<BaseOpAST> l_or_op;
    std::unique_ptr<BaseExpAST> l_and_exp;
};

class LOrExpAST : public BaseExpAST
{
public:
    union
    {
        LOrExpCase0 *l_or_exp_case_0;
        LOrExpCase1 *l_or_exp_case_1;
    } l_or_exp_union;
    void Dump(int ctl = 0) override;
    LOrExpAST(int __tag__);
};

#endif