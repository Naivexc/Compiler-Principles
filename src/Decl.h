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

#endif