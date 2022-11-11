#include "LAndOp.h"

void LAndOpAST::Dump(int ctl)
{
    int temp_val = 0;
    PRINT_DUMP("LAndOp", DEBUG_BEGIN);
    temp_val = lhs.val && rhs.val;
    if (is_calculating_const_exp == false)
    {

        k_str += "    %";
        k_str += itostr(unused_koopa_count);
        k_str += " = ne ";
        if (lhs.is_num == true)
        {
            k_str += itostr(lhs.val);
        }
        else
        {
            k_str += '%';
            k_str += itostr(lhs.count);
        }
        k_str += ", 0\n";
        ++unused_koopa_count;
        if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        k_str += "    %";
        k_str += itostr(unused_koopa_count);
        k_str += " = ne ";
        if (rhs.is_num == true)
        {
            k_str += itostr(rhs.val);
        }
        else
        {
            k_str += '%';
            k_str += itostr(rhs.count);
        }
        k_str += ", 0\n";
        ++unused_koopa_count;
        if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        k_str += "    %";
        k_str += itostr(unused_koopa_count);
        k_str += " = and %";
        k_str += itostr(unused_koopa_count - 1);
        k_str += ", %";
        k_str += itostr(unused_koopa_count - 2);
        k_str += '\n';
        result.is_num = false;
        result.count = unused_koopa_count;
        ++unused_koopa_count;
        if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
    }
    result.val = temp_val;
    PRINT_DUMP("LAndOp", DEBUG_END);
}