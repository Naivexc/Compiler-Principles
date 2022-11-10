#include "FuncFParam.h"

std::string FuncFParamCase1::return_type()
{
    std::string arr_type = "i32";
    int n = vec_const_exp.size();
    for (int i = 0; i < n; ++i)
    {
        arr_type = ("[" + arr_type + ", " + itostr(vec_const_exp[n - 1 - i]->data.val) + ']');
    }
    arr_type = "*" + arr_type;
    return arr_type;
}