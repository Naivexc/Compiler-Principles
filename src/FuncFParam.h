#ifndef FUNC_F_PARAM_H
#define FUNC_F_PARAM_H

#include "ast.h"

class FuncFParamCase0
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::string ident;
};

class FuncFParamCase1
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_const_exp;
    std::string return_type();
};

class FuncFParamAST : public BaseAST
{
public:
    union
    {
        FuncFParamCase0 *func_f_param_case_0;
        FuncFParamCase1 *func_f_param_case_1;
    } func_f_param_union;
    int size;
    std::string koopa_type;
    FuncFParamAST(int __tag__, std::string __ident__ = "");
    void Dump(int ctl = 0) override;
    void declare_alloc();
};

#endif