#include "EqExp.h"

void EqExpAST::Dump(int ctl)
{
    PRINT_DUMP("EqExp", DEBUG_BEGIN);
    switch (tag)
    {
    case 0:
        ++depth_dump;
        eq_exp_union.eq_exp_case_0->rel_exp->Dump();
        data = eq_exp_union.eq_exp_case_0->rel_exp->data;
        --depth_dump;
        break;
    case 1:
        ++depth_dump;
        eq_exp_union.eq_exp_case_1->rel_exp->Dump();
        eq_exp_union.eq_exp_case_1->eq_op->rhs = eq_exp_union.eq_exp_case_1->rel_exp->data;
        --depth_dump;
        ++depth_dump;
        eq_exp_union.eq_exp_case_1->eq_exp->Dump();
        eq_exp_union.eq_exp_case_1->eq_op->lhs = eq_exp_union.eq_exp_case_1->eq_exp->data;
        --depth_dump;
        ++depth_dump;
        eq_exp_union.eq_exp_case_1->eq_op->Dump();
        data = eq_exp_union.eq_exp_case_1->eq_op->result;
        break;
    default:
        assert(false);
    }
    PRINT_DUMP("EqExp", DEBUG_END);
}
EqExpAST::EqExpAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        eq_exp_union.eq_exp_case_0 = new EqExpCase0();
        break;
    case 1:
        eq_exp_union.eq_exp_case_1 = new EqExpCase1();
        break;
    default:
        assert(false);
    }
    return;
}