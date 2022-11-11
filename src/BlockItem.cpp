#include "BlockItem.h"

void BlockItemAST::Dump(int ctl = 0)
{
    PRINT_DUMP("BlockItem", DEBUG_BEGIN);
    if (last_ins_is_ret == true)
    {
        k_str += ("  %block_" + itostr(unused_koopa_label_count) + ":\n");
        ++unused_koopa_label_count;
        last_ins_is_ret = false;
    }
    ++depth_dump;
    if (tag == 0)
        decl->Dump();
    else
        stmt->Dump();
    --depth_dump;
    PRINT_DUMP("BlockItem", DEBUG_END);
}