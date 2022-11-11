#ifndef FUNC_R_PARAMS
#define FUNC_R_PARAMS

#include "Base.h"
#include "Exp.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <vector>
#include <string>

extern std::string k_str;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;

class FuncRParamsAST : public BaseAST
{
public:
    std::vector<ExpAST *> func_r_params;
    void Dump(int ctl = 0) override;
};

#endif