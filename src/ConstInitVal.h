#ifndef CONST_INIT_VAL_H
#define CONST_INIT_VAL_H

#include "Base.h"
#include "InitList.h"
#include <memory>
#include <vector>

extern bool is_calculating_const_exp;

class ConstInitValCase0
{
public:
    std::unique_ptr<BaseExpAST> const_exp;
    CalData data;
};

class ConstInitValCase1
{
public:
    std::vector<std::unique_ptr<BaseAST>> vec_const_init_val;
};

class ConstInitValAST : public BaseAST
{
public:
    union
    {
        ConstInitValCase0 *const_init_val_case_0;
        ConstInitValCase1 *const_init_val_case_1;
    } const_init_val_union;
    void Dump(int ctl = 0) override;
    ConstInitValAST(int __tag__);
};

extern void ConstInitValInit(std::vector<std::unique_ptr<BaseExpAST>>::iterator iter_begin, std::vector<std::unique_ptr<BaseExpAST>>::iterator iter_end, InitList *civ);

#endif