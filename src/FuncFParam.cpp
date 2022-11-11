#include "FuncFParam.h"
#include <cassert>

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

FuncFParamAST::FuncFParamAST(int __tag__, std::string __ident__)
{
    tag = __tag__;
    switch (tag)
    {
    case (0):
    {
        func_f_param_union.func_f_param_case_0 = new FuncFParamCase0();
        func_f_param_union.func_f_param_case_0->ident = __ident__;
        break;
    }
    case (1):
    {
        func_f_param_union.func_f_param_case_1 = new FuncFParamCase1();
        func_f_param_union.func_f_param_case_1->ident = __ident__;
        break;
    }
    default:
        assert(false);
    }
};

void FuncFParamAST::Dump(int ctl)
{
    switch (tag)
    {
    case (0):
    {
        k_str += (std::string("%") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + ": ");
        if (func_f_param_union.func_f_param_case_0->b_type->type == "int")
        {
            koopa_type = "i32";
            k_str += koopa_type;
            size = 4;
        }
        else
            assert(false);
        break;
    }
    case (1):
    {
        size = 4;
        for (int i = 0; i < func_f_param_union.func_f_param_case_1->vec_const_exp.size(); ++i)
        {
            is_calculating_const_exp = true;
            func_f_param_union.func_f_param_case_1->vec_const_exp[i]->Dump();
            is_calculating_const_exp = false;
        }
        koopa_type = func_f_param_union.func_f_param_case_1->return_type();
        k_str += (std::string("%") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + ": " + koopa_type);
        break;
    }
    default:
        assert(false);
    }
}
void FuncFParamAST::declare_alloc()
{
    switch (tag)
    {
    case (0):
    {
        cur_map_iter_for_func_space_needed->second.variate_needed += size;
        cur_symbol_table->insert(func_f_param_union.func_f_param_case_0->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + koopa_type, STRONG));
        k_str += ("    " + func_f_param_union.func_f_param_case_0->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + koopa_type + "\n");
        k_str += (std::string("    store %") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + std::string(", ") + func_f_param_union.func_f_param_case_0->ident.c_str() + '_' + itostr(unused_koopa_var_count) + '\n');
        ++unused_koopa_var_count;
        break;
    }
    case (1):
    {
        cur_map_iter_for_func_space_needed->second.variate_needed += size;
        cur_symbol_table->insert(func_f_param_union.func_f_param_case_1->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + koopa_type, STRONG));
        k_str += ("    " + func_f_param_union.func_f_param_case_1->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + koopa_type + "\n");
        k_str += (std::string("    store %") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + std::string(", ") + func_f_param_union.func_f_param_case_1->ident.c_str() + '_' + itostr(unused_koopa_var_count) + '\n');
        ++unused_koopa_var_count;
        break;
    }
    default:
        assert(false);
    }
}