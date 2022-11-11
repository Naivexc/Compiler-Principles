#include "LOrOp.h"

void LOrOpAST::Dump(int ctl = 0)
{
    PRINT_DUMP("LOrOp", DEBUG_BEGIN);
    result.val = lhs.val || rhs.val;
    if (is_calculating_const_exp == false)
    {
        k_str += "    %";
        k_str += itostr(unused_koopa_count);
        k_str += " = or ";
        if (lhs.is_num == true)
        {
            k_str += itostr(lhs.val);
        }
        else
        {
            k_str += '%';
            k_str += itostr(lhs.count);
        }
        k_str += ", ";
        if (rhs.is_num == true)
        {
            k_str += itostr(rhs.val);
        }
        else
        {
            k_str += '%';
            k_str += itostr(rhs.count);
        }
        k_str += "\n";
        result.is_num = false;
        result.count = unused_koopa_count;
        ++unused_koopa_count;
        if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        k_str += "    %";
        k_str += itostr(unused_koopa_count);
        k_str += " = ne %";
        k_str += itostr(unused_koopa_count - 1);
        k_str += ", ";
        k_str += itostr(0);
        k_str += '\n';
        result.count = unused_koopa_count;
        ++unused_koopa_count;
        if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
    }

    PRINT_DUMP("LOrOp", DEBUG_END);
}