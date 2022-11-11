#include "CompUnitEle.h"
#include <cassert>

void CompUnitEle::Dump(int ctl)
{
    if (tag == 0)
        comp_unit_ele_union.comp_unit_ele_case_0->func_def->Dump();
    else
        comp_unit_ele_union.comp_unit_ele_case_1->decl->Dump();
}
CompUnitEle::CompUnitEle(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case 0:
        comp_unit_ele_union.comp_unit_ele_case_0 = new CompUnitEleCase0();
        break;
    case 1:
        comp_unit_ele_union.comp_unit_ele_case_1 = new CompUnitEleCase1();
        break;
    default:
        assert(false);
    }
    return;
}