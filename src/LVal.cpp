#include "LVal.h"
#include "Debug.h"
#include "SymbolTable.h"
extern SymbolTable *cur_symbol_table;
void LValAST::Dump(int ctl)
{

    PRINT_DUMP("LVal", DEBUG_BEGIN);
    ++depth_dump;
    switch (tag)
    {
    case (0):
    {
        auto iter = cur_symbol_table->find(l_val_union.l_val_case_0->ident);
        if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
        {
            iter = cur_symbol_table->find(std::string("%") + (l_val_union.l_val_case_0->ident.c_str() + 1));
            if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
            {
                printf("%s: not found\n", l_val_union.l_val_case_0->ident.c_str());
                printf("%s\n", k_str.c_str());
                assert(false);
            }
        }
        if (iter->second->data.var->flag_is_const == IS_CONST)
        {
            data.is_num = true;
            data.count = -1;
            data.val = iter->second->data.var->val;
        }
        else if (iter->second->data.var->var_type == "*i32" || iter->second->data.var->var_type.substr(0, 2) == "**")
        {
            k_str += ("    %" + itostr(unused_koopa_count) + " = load " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + '\n');
            data.is_num = false;
            data.count = unused_koopa_count;
            ++unused_koopa_count;
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        }
        else if (iter->second->data.var->var_type.substr(0, 2) == "*[")
        {
            k_str += ("    %" + itostr(unused_koopa_count) + " = getelemptr " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + ", 0\n");
            data.is_num = false;
            data.count = unused_koopa_count;
            ++unused_koopa_count;
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        }
        else
        {
            printf("%s\n", iter->second->data.var->var_type.c_str());
            assert(false);
        }
        break;
    }
    case (1):
    {
        auto iter = cur_symbol_table->find(l_val_union.l_val_case_1->ident);
        for (int i = 0; i < l_val_union.l_val_case_1->vec_exp.size(); ++i)
            l_val_union.l_val_case_1->vec_exp[i]->Dump();
        if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
        {
            iter = cur_symbol_table->find(std::string("%") + (l_val_union.l_val_case_0->ident.c_str() + 1));
            if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
                assert(false);
        }
        if (iter->second->data.var->var_type.substr(0, 2) == "**")
        {
            k_str += ("    %" + itostr(unused_koopa_count) + " = load " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + '\n');
            ++unused_koopa_count;
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        }
        if (iter->second->data.var->var_type.substr(0, 2) == "*[")
            k_str += "    %" + itostr(unused_koopa_count) + " = getelemptr " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + ", ";
        else if (iter->second->data.var->var_type.substr(0, 2) == "**")
            k_str += "    %" + itostr(unused_koopa_count) + " = getptr %" + itostr(unused_koopa_count - 1) + ", ";
        else
        {
            printf("%s\n", iter->second->data.var->var_type.c_str());
            assert(false);
        }
        l_val_union.l_val_case_1->vec_exp[0]->data.print_data_to_k_str();
        k_str += '\n';
        ++unused_koopa_count;
        cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");

        for (int i = 1; i < l_val_union.l_val_case_1->vec_exp.size(); ++i)
        {
            k_str += "    %" + itostr(unused_koopa_count) + " = getelemptr %" + itostr(unused_koopa_count - 1) + ", ";
            l_val_union.l_val_case_1->vec_exp[i]->data.print_data_to_k_str();
            k_str += '\n';
            ++unused_koopa_count;
            cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        }
        if (iter->second->data.var->dim == l_val_union.l_val_case_1->vec_exp.size() + 1)
            k_str += ("    %" + itostr(unused_koopa_count) + " = load " + "%" + itostr(unused_koopa_count - 1) + '\n');
        else
            k_str += ("    %" + itostr(unused_koopa_count) + " = getelemptr %" + itostr(unused_koopa_count - 1) + ", 0\n");
        data.is_num = false;
        data.count = unused_koopa_count;
        ++unused_koopa_count;
        cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        break;
    }
    default:
    {
        assert(false);
        break;
    }
    }
    --depth_dump;
    PRINT_DUMP("LVal", DEBUG_END);
}
LValAST::LValAST(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        l_val_union.l_val_case_0 = new LValCase0();
        break;
    case 1:
        l_val_union.l_val_case_1 = new LValCase1();
        break;
    default:
        assert(false);
    }
    return;
}