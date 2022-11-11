#include "FuncType.h"

void FuncTypeAST::Dump(int ctl = 0)
{
    PRINT_DUMP("FuncType", DEBUG_BEGIN);
    if (type == "int")
        k_str += ": i32\n";
    else if (type == "void")
        k_str += '\n';
    PRINT_DUMP("FuncType", DEBUG_END);
}