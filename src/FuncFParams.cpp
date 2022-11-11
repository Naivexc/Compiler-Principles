#include "FuncFParams.h"
#include <vector>

void FuncFParamsAST::Dump(int ctl)
{
    if (func_f_params.empty())
        return;
    std::vector<FuncFParamAST *>::iterator iter = func_f_params.begin();
    (*iter)->Dump();
    ++iter;
    while (iter != func_f_params.end())
    {
        k_str += ',';
        (*iter)->Dump();
        ++iter;
    }
}
void FuncFParamsAST::declare_alloc()
{
    auto iter = func_f_params.begin();
    while (iter != func_f_params.end())
    {
        (*iter)->declare_alloc();
        ++iter;
    }
}