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

#endif