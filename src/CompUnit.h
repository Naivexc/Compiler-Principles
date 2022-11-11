#include "Base.h"

class CompUnitAST : public BaseAST
{
public:
    std::unique_ptr<CompUnitEle> comp_unit_ele;
    std::unique_ptr<BaseAST> next_comp_unit_ele;
    void Dump(int ctl = 0) override;
};