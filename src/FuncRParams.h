#ifndef FUNC_R_PARAMS
#define FUNC_R_PARAMS

#include "Base.h"
#include "Exp.h"

class FuncRParamsAST : public BaseAST
{
public:
    std::vector<ExpAST *> func_r_params;
    void Dump(int ctl = 0) override;
};

#endif