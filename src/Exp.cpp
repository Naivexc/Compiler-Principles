#include "Exp.h"

void ExpAST::Dump(int ctl = 0)
{
    PRINT_DUMP("Exp", DEBUG_BEGIN);
    ++depth_dump;
    if (cur_symbol_table == symbol_table_tree.symbol_table_tree_root.get())
        is_calculating_const_exp = true;
    l_or_exp->Dump();
    is_calculating_const_exp = false;
    data = l_or_exp->data;
    --depth_dump;
    PRINT_DUMP("Exp", DEBUG_END);
}