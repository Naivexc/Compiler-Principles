#ifndef COMP_UNIT_ELE_H
#define COMP_UNIT_ELE_H

#include "ast.h"

class CompUnitEleCase0
{
public:
    std::unique_ptr<BaseAST> func_def;
};

class CompUnitEleCase1
{
public:
    std::unique_ptr<BaseAST> decl;
};

#endif