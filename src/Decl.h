#ifndef DECL_H
#define DECL_H

#include "ast.h"

class DeclCase0
{
public:
    std::unique_ptr<BaseAST> const_decl;
};

class DeclCase1
{
public:
    std::unique_ptr<BaseAST> var_decl;
};

class DeclAST : public BaseAST
{
public:
    union
    {
        DeclCase0 *decl_case_0;
        DeclCase1 *decl_case_1;
    } decl_union;
    void Dump(int ctl = 0) override;
    DeclAST(int __tag__);
};

#endif