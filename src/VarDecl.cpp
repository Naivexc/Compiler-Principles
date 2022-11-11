#include "VarDecl.h"

void VarDeclAST::Dump(int ctl = 0)
{
    PRINT_DUMP("VarDecl", DEBUG_BEGIN);
    ++depth_dump;
    b_type->Dump();
    --depth_dump;
    ++depth_dump;
    for (auto iter = vec_var_def.begin(); iter != vec_var_def.end(); ++iter)
    {
        (*iter)->Dump();
    }
    --depth_dump;
    PRINT_DUMP("VarDecl", DEBUG_END);
}