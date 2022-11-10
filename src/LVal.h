#ifndef L_VAL_H
#define L_VAL_H

#include "ast.h"

class LValCase0
{
public:
    std::string ident;
};

class LValCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_exp;
};

#endif