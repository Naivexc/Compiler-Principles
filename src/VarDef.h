#ifndef VAR_DEF_H
#define VAR_DEF_H

#include "ast.h"

class VarDefCase0
{
public:
    std::string ident;
};

class VarDefCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_const_exp;
    std::unique_ptr<BaseAST> init_val;
    std::string return_type();
};

class VarDefCase2
{
public:
    std::string ident;
    std::unique_ptr<BaseAST> init_val;
};

#endif