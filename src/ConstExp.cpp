#include "ConstExp.h"

void ConstExpAST::Dump(int ctl)
{
    PRINT_DUMP("ConstExp", DEBUG_BEGIN);
    ++depth_dump;
    exp->Dump();
    data = exp->data;
    --depth_dump;
    PRINT_DUMP("ConstExp", DEBUG_END);
}