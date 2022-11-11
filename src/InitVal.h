#ifndef INIT_VAL_H
#define INIT_VAL_H

#include "Base.h"
#include "CalData.h"
#include <memory>
#include <cassert>
#include <vector>

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

class InitValAST : public BaseAST
{
public:
    union
    {
        InitValCase0 *init_val_case_0;
        InitValCase1 *init_val_case_1;
    } init_val_union;
    void Dump(int ctl = 0) override;
    InitValAST(int __tag__);
};

#endif