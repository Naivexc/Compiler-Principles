#include "FuncDef.h"

void FuncDefAST::Dump(int ctl = 0)
{
    PRINT_DUMP("FuncDef", DEBUG_BEGIN);
    last_ins_is_ret = false;
    auto temp_symbol_table = cur_symbol_table;
    cur_symbol_table->insert(ident, Function(dynamic_cast<FuncTypeAST *>(func_type.get())->type, dynamic_cast<FuncFParamsAST *>(func_f_params.get())->func_f_params));
    cur_symbol_table = symbol_table_tree.symbol_table_tree_root->AddSymbolTable();
    cur_map_iter_for_func_space_needed = map_space_needed_for_function.insert(make_pair(ident, SpaceNeeded())).first;
    k_str += "fun ";
    k_str += ident;
    k_str += '(';
    func_f_params->Dump();
    k_str += ")";
    ++depth_dump;
    func_type->Dump();
    --depth_dump;
    k_str += "{\n";
    k_str += "  %entry:\n";
    dynamic_cast<FuncFParamsAST *>(func_f_params.get())->declare_alloc();
    ++depth_dump;
    block->Dump();
    if (last_ins_is_ret == false)
    {
        if (dynamic_cast<FuncTypeAST *>(func_type.get())->type == "void")
            k_str += "    ret\n";
        else
            k_str += "    ret 0\n";
    }
    --depth_dump;
    k_str += "}\n";
    cur_symbol_table = temp_symbol_table;
    int S = cur_map_iter_for_func_space_needed->second.variate_needed;
    int R = cur_map_iter_for_func_space_needed->second.return_address_needed;
    int A = cur_map_iter_for_func_space_needed->second.params_needed;
    cur_map_iter_for_func_space_needed->second.total_needed = (S + R + A + 15) / 16;
    cur_map_iter_for_func_space_needed->second.total_needed *= 16;
    PRINT_DUMP("FuncDef", DEBUG_END);
}