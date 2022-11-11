#ifndef UNARY_EXP_H
#define UNARY_EXP_H
#include "Debug.h"
#include "Base.h"
#include <memory>
struct UnaryExpCase0
{
    std::unique_ptr<BaseExpAST> primary_exp;
};

struct UnaryExpCase1
{
    std::string ident;
    std::unique_ptr<BaseAST> func_r_params;
};

struct UnaryExpCase2
{
    std::unique_ptr<BaseOpAST> unary_op;
    std::unique_ptr<BaseExpAST> unary_exp;
};

class UnaryExpAST : public BaseExpAST
{
public:
    // tag 0
    union
    {
        UnaryExpCase0 *unary_exp_case_0;
        UnaryExpCase1 *unary_exp_case_1;
        UnaryExpCase2 *unary_exp_case_2;
    } unary_exp_union;
    void Dump(int ctl = 0) override;
    UnaryExpAST(int __tag__)
    {
        tag = __tag__;
        switch (tag)
        {
        case 0:
            unary_exp_union.unary_exp_case_0 = new UnaryExpCase0();
            break;
        case 1:
            unary_exp_union.unary_exp_case_1 = new UnaryExpCase1();
            break;
        case 2:
            unary_exp_union.unary_exp_case_2 = new UnaryExpCase2();
            break;
        default:
            assert(false);
        }
        return;
    }
};
#endif