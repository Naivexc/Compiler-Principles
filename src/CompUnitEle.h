#ifndef COMP_UNIT_ELE_H
#define COMP_UNIT_ELE_H

#include "Base.h"
#include <memory>

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

class CompUnitEle : public BaseAST
{
public:
    union
    {
        CompUnitEleCase0 *comp_unit_ele_case_0;
        CompUnitEleCase1 *comp_unit_ele_case_1;
    } comp_unit_ele_union;
    void Dump(int ctl = 0) override;
    CompUnitEle(int __tag__);
};

#endif