#include "UnaryExp.h"
#include "Debug.h"
void UnaryExpAST::Dump(int ctl = 0) // override
{
    PRINT_DUMP("UnaryExp", DEBUG_BEGIN);
    switch (tag)
    {
    case 0: // primaryexp
    {
        ++depth_dump;
        unary_exp_union.unary_exp_case_0->primary_exp->Dump();
        data = unary_exp_union.unary_exp_case_0->primary_exp->data;
        --depth_dump;
        break;
    }
    case 1:
    {
        auto iter = cur_symbol_table->find(unary_exp_union.unary_exp_case_1->ident);
        if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end())
        {
            symbol_table_tree.symbol_table_tree_root->print_symbol_table();
            cur_symbol_table->father_symbol_table->print_symbol_table();
            assert(false);
        }
        if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 1)
            assert(false);
        cur_map_iter_for_func_space_needed->second.return_address_needed = 4;
        if (iter->second->data.func->ret_type == "void")
        {
            unary_exp_union.unary_exp_case_1->func_r_params->Dump(1);
            k_str += ("    call " + iter->first + "(");
            unary_exp_union.unary_exp_case_1->func_r_params->Dump();
            k_str += ")\n";
            data.count = -1;
            data.is_num = false;
            data.val = -1;
        }
        else
        {
            unary_exp_union.unary_exp_case_1->func_r_params->Dump(1);
            data.count = unused_koopa_count;
            ++unused_koopa_count;
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
            data.is_num = false;
            data.val = -1;
            k_str += ("    %" + itostr(data.count) + " = call " + iter->first + '(');
            unary_exp_union.unary_exp_case_1->func_r_params->Dump();
            k_str += ")\n";
        }
        break;
    }
    case 2: // unary
    {
        ++depth_dump;
        unary_exp_union.unary_exp_case_2->unary_exp->Dump();
        unary_exp_union.unary_exp_case_2->unary_op->rhs = unary_exp_union.unary_exp_case_2->unary_exp->data;
        --depth_dump;
        ++depth_dump;
        unary_exp_union.unary_exp_case_2->unary_op->Dump();
        data = unary_exp_union.unary_exp_case_2->unary_op->result;
        --depth_dump;
        break;
    }
    default:
        assert(false);
    }
    PRINT_DUMP("UnaryExp", DEBUG_END);
}