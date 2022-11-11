#include "InitVal.h"

void InitValAST::Dump(int ctl = 0)
{
    PRINT_DUMP("InitVal", DEBUG_BEGIN);
    ++depth_dump;
    switch (tag)
    {
    case (0):
    {
        init_val_union.init_val_case_0->exp->Dump();
        init_val_union.init_val_case_0->data = init_val_union.init_val_case_0->exp->data;
        break;
    }
    case (1):
    {
        for (int i = 0; i < init_val_union.init_val_case_1->vec_init_val.size(); ++i)
        {
            ++depth_dump;
            init_val_union.init_val_case_1->vec_init_val[i]->Dump();
            --depth_dump;
        }
        break;
    }
    default:
        assert(false);
        break;
    }
    --depth_dump;
    PRINT_DUMP("InitVal", DEBUG_END);
}
InitValAST::InitValAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        init_val_union.init_val_case_0 = new InitValCase0();
        break;
    case 1:
        init_val_union.init_val_case_1 = new InitValCase1();
        break;
    default:
        assert(false);
    }
    return;
}