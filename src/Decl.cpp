#include "Decl.h"

void DeclAST::Dump(int ctl)
{
    PRINT_DUMP("Decl", DEBUG_BEGIN);
    ++depth_dump;
    switch (tag)
    {
    case (0):
    {
        is_calculating_const_exp = true;
        if (is_calculating_const_exp == false)
            assert(false);
        decl_union.decl_case_0->const_decl->Dump();
        is_calculating_const_exp = false;
        break;
    }
    case (1):
    {
        is_calculating_const_exp = false;
        decl_union.decl_case_1->var_decl->Dump();
        break;
    }
    }
    --depth_dump;
    PRINT_DUMP("Decl", DEBUG_END);
}
DeclAST::DeclAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        decl_union.decl_case_0 = new DeclCase0();
        break;
    case 1:
        decl_union.decl_case_1 = new DeclCase1();
        break;
    default:
        assert(false);
    }
    return;
}