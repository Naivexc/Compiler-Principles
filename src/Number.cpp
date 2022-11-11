#include "Number.h"
void NumberAST::Dump(int ctl)
{
    PRINT_DUMP("Number", DEBUG_BEGIN);
    data.is_num = true;
    data.val = atoi(num.c_str());
    data.count = -1;
    PRINT_DUMP("Number", DEBUG_END);
}