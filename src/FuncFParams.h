#ifndef FUNC_F_PARAMS
#define FUNC_F_PARAMS

#include "Base.h"
#include "FuncFParam.h"

extern std::string k_str;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;

class FuncFParamsAST : public BaseAST
{
public:
    std::vector<FuncFParamAST *> func_f_params;
    void Dump(int ctl = 0) override;
    void declare_alloc();
};

#endif