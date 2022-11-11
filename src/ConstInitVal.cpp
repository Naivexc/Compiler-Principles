#include "ConstInitVal.h"

void ConstInitValAST::Dump(int ctl = 0)
{
    PRINT_DUMP("ConstInitVal", DEBUG_BEGIN);
    switch (tag)
    {
    case (0):
    {
        ++depth_dump;
        is_calculating_const_exp = true;
        const_init_val_union.const_init_val_case_0->const_exp->Dump();
        is_calculating_const_exp = false;
        const_init_val_union.const_init_val_case_0->data = const_init_val_union.const_init_val_case_0->const_exp->data;
        --depth_dump;
        break;
    }
    case (1):
    {
        for (int i = 0; i < const_init_val_union.const_init_val_case_1->vec_const_init_val.size(); ++i)
        {
            ++depth_dump;
            const_init_val_union.const_init_val_case_1->vec_const_init_val[i]->Dump();
            // const_init_val_union.const_init_val_case_1->vec_data.push_back(const_init_val_union.const_init_val_case_1->vec_const_exp[i]->data);
            --depth_dump;
        }
        break;
    }
    default:
        assert(false);
        break;
    }
    PRINT_DUMP("ConstInitVal", DEBUG_END);
}
ConstInitValAST::ConstInitValAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        const_init_val_union.const_init_val_case_0 = new ConstInitValCase0();
        const_init_val_union.const_init_val_case_0->data.val = 0;
        const_init_val_union.const_init_val_case_0->data.is_num = true;
        break;
    case 1:
        const_init_val_union.const_init_val_case_1 = new ConstInitValCase1();
        break;
    default:
        assert(false);
    }
    return;
}