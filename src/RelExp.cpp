#include "RelExp.h"

void RelExp::Dump(int ctl = 0)
{
    PRINT_DUMP("RelExp", DEBUG_BEGIN);
    switch (tag)
    {
    case (0):
    {
        ++depth_dump;
        rel_exp_union.rel_exp_case_0->add_exp->Dump();
        data = rel_exp_union.rel_exp_case_0->add_exp->data;
        --depth_dump;
        break;
    }
    case (1):
    {
        ++depth_dump;
        rel_exp_union.rel_exp_case_1->add_exp->Dump();
        rel_exp_union.rel_exp_case_1->rel_op->rhs = rel_exp_union.rel_exp_case_1->add_exp->data;
        --depth_dump;
        ++depth_dump;
        rel_exp_union.rel_exp_case_1->rel_exp->Dump();
        rel_exp_union.rel_exp_case_1->rel_op->lhs = rel_exp_union.rel_exp_case_1->rel_exp->data;
        --depth_dump;
        ++depth_dump;
        rel_exp_union.rel_exp_case_1->rel_op->Dump();
        data = rel_exp_union.rel_exp_case_1->rel_op->result;
        break;
    }
    default:
    {
        assert(false);
        break;
    }
    }
    PRINT_DUMP("RelExp", DEBUG_END);
}
RelExp::RelExpAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        rel_exp_union.rel_exp_case_0 = new RelExpCase0();
        break;
    case 1:
        rel_exp_union.rel_exp_case_1 = new RelExpCase1();
        break;
    default:
        assert(false);
    }
    return;
}