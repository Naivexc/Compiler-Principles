#include "Base.h"

class FuncFParamsAST : public BaseAST
{
public:
    std::vector<FuncFParamAST *> func_f_params;
    void Dump(int ctl = 0) override;
    void declare_alloc();
};