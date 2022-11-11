#include "Block.h"

void BlockAST::Dump(int ctl)
{
    PRINT_DUMP("Block", DEBUG_BEGIN);
    for (auto iter = vec_block_item.begin(); iter != vec_block_item.end(); ++iter)
    {
        ++depth_dump;
        (*iter)->Dump();
        --depth_dump;
    }
    PRINT_DUMP("Block", DEBUG_END);
}