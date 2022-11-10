#ifndef INIT_VAL_H
#define INIT_VAL_H

#include "ast.h"

class InitValCase0
{
public:
    std::unique_ptr<BaseExpAST> exp;
    CalData data;
};

class InitValCase1
{
public:
    std::vector<std::unique_ptr<BaseAST>> vec_init_val;
    // std::vector<CalData> vec_data;
};

#endif