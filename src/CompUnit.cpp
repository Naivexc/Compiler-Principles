#include "CompUnit.h"

void CompUnitAST::Dump(int ctl = 0)
{
    PRINT_DUMP("CompUnit", DEBUG_BEGIN);
    ++depth_dump;
    comp_unit_ele->Dump();
    if (next_comp_unit_ele != NULL)
        next_comp_unit_ele->Dump();
    --depth_dump;
    PRINT_DUMP("CompUnit", DEBUG_END);
}