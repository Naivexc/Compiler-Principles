#include "Number.h"
void NumberAST::Dump(int ctl = 0)
{
    PRINT_DUMP("Number", DEBUG_BEGIN);
    data.is_num = true;
    data.val = atoi(num.c_str());
    data.count = -1;
    PRINT_DUMP("Number", DEBUG_END);
}