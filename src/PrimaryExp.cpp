#include "PrimaryExp.h"

void PrimaryExpAST::Dump(int ctl)
{
    PRINT_DUMP("PrimaryExp", DEBUG_BEGIN);
    switch (tag)
    {
    case 0: // primaryexp
    {
        ++depth_dump;
        primary_exp_union.primary_exp_case_0->exp->Dump();
        data = primary_exp_union.primary_exp_case_0->exp->data;
        --depth_dump;
        break;
    }
    case 1:
    {
        ++depth_dump;
        primary_exp_union.primary_exp_case_1->l_val->Dump();
        --depth_dump;
        data = dynamic_cast<LValAST *>(primary_exp_union.primary_exp_case_1->l_val.get())->data;
        break;
    }
    case 2: // unary
    {
        ++depth_dump;
        primary_exp_union.primary_exp_case_2->number->Dump();
        data = primary_exp_union.primary_exp_case_2->number->data;
        --depth_dump;
        break;
    }
    default:
    {
        assert(false);
    }
    }
    PRINT_DUMP("PrimaryExp", DEBUG_END);
}
PrimaryExpAST::PrimaryExpAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        primary_exp_union.primary_exp_case_0 = new PrimaryExpCase0();
        break;
    case 1:
        primary_exp_union.primary_exp_case_1 = new PrimaryExpCase1();
        break;
    case 2:
        primary_exp_union.primary_exp_case_2 = new PrimaryExpCase2();
        break;
    default:
        assert(false);
    }
    return;
}