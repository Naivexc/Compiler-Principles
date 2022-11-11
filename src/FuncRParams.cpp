#include "FuncRParams.h"

void FuncRParamsAST::Dump(int ctl = 0)
{
    if (ctl == 0)
    {
        if (func_r_params.empty())
            return;
        std::vector<ExpAST *>::iterator iter = func_r_params.begin();
        (*iter)->data.print_data_to_k_str();
        ++iter;
        while (iter != func_r_params.end())
        {
            k_str += ',';
            (*iter)->data.print_data_to_k_str();
            ++iter;
        }
    }
    else
    {
        cur_map_iter_for_func_space_needed->second.params_needed = max_int(cur_map_iter_for_func_space_needed->second.params_needed, (func_r_params.size() - 8) * 4);
        if (func_r_params.empty())
            return;
        std::vector<ExpAST *>::iterator iter = func_r_params.begin();
        while (iter != func_r_params.end())
        {
            (*iter)->Dump();
            ++iter;
        }
    }
}