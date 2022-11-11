#include "MulExp.h"

void MulExpAST::Dump(int ctl)
{
    PRINT_DUMP("MulExp", DEBUG_BEGIN);
    switch (tag)
    {
    case 0:
        // unaryexp
        ++depth_dump;
        mul_exp_union.mul_exp_case_0->unary_exp->Dump();
        data = mul_exp_union.mul_exp_case_0->unary_exp->data;
        --depth_dump;
        break;
    case 1:
        ++depth_dump;
        mul_exp_union.mul_exp_case_1->unary_exp->Dump();
        mul_exp_union.mul_exp_case_1->mul_op.get()->rhs = mul_exp_union.mul_exp_case_1->unary_exp->data;
        --depth_dump;
        ++depth_dump;
        mul_exp_union.mul_exp_case_1->mul_exp->Dump();
        mul_exp_union.mul_exp_case_1->mul_op.get()->lhs = mul_exp_union.mul_exp_case_1->mul_exp->data;
        --depth_dump;
        ++depth_dump;
        mul_exp_union.mul_exp_case_1->mul_op->Dump();
        data = mul_exp_union.mul_exp_case_1->mul_op->result;
        --depth_dump;
        break;
    default:
        assert(false);
    }
    PRINT_DUMP("MulExp", DEBUG_END);
}
MulExpAST::MulExpAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        mul_exp_union.mul_exp_case_0 = new MulExpCase0();
        break;
    case 1:
        mul_exp_union.mul_exp_case_1 = new MulExpCase1();
        break;
    default:
        assert(false);
    }
    return;
}