#ifndef PRIMARY_EXP_H
#define PRIMARY_EXP_H
#include "ast.h"

class PrimaryExpCase0
{
public:
    std::unique_ptr<BaseExpAST> exp;
};

class PrimaryExpCase1
{
public:
    std::unique_ptr<BaseAST> l_val;
};

class PrimaryExpCase2
{
public:
    std::unique_ptr<BaseExpAST> number;
};
#endif