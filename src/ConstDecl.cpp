#include "ConstDecl.h"

void ConstDeclAST::Dump(int ctl)
{
    PRINT_DUMP("ConstDecl", DEBUG_BEGIN);
    ++depth_dump;
    b_type->Dump();
    --depth_dump;
    ++depth_dump;
    if (is_calculating_const_exp == false)
        assert(false);
    for (auto iter = vec_const_def.begin(); iter != vec_const_def.end(); ++iter)
    {
        (*iter)->Dump();
    }
    --depth_dump;
    PRINT_DUMP("ConstDecl", DEBUG_END);
}