#include "AddExp.h"
#include <cassert>
void AddExpAST::Dump(int ctl)
{
    PRINT_DUMP("AddExp", DEBUG_BEGIN);
    switch (tag)
    {
    case (0):
    {
        // unaryexp
        ++depth_dump;
        add_exp_union.add_exp_case_0->mul_exp->Dump();
        data = add_exp_union.add_exp_case_0->mul_exp->data;
        --depth_dump;
        break;
    }
    case (1):
    {
        ++depth_dump;
        add_exp_union.add_exp_case_1->mul_exp->Dump();
        add_exp_union.add_exp_case_1->add_op->rhs = add_exp_union.add_exp_case_1->mul_exp->data;
        --depth_dump;
        ++depth_dump;
        add_exp_union.add_exp_case_1->add_exp->Dump();
        add_exp_union.add_exp_case_1->add_op->lhs = add_exp_union.add_exp_case_1->add_exp->data;
        --depth_dump;
        ++depth_dump;
        add_exp_union.add_exp_case_1->add_op->Dump();
        data = add_exp_union.add_exp_case_1->add_op->result;
        --depth_dump;
        break;
    }
    }
    PRINT_DUMP("AddExp", DEBUG_END);
}
AddExpAST::AddExpAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        add_exp_union.add_exp_case_0 = new AddExpCase0();
        break;
    case 1:
        add_exp_union.add_exp_case_1 = new AddExpCase1();
        break;
    default:
        assert(false);
    }
    return;
}