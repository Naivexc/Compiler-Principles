#ifndef FUNC_DEF_H
#define FUNC_DEF_H

#include "Base.h"
#include "FuncType.h"
#include "FuncFParam.h"
#include "FuncFParams.h"
#include "SpaceNeeded.h"
#include <map>
#include <string>

extern bool last_ins_is_ret;
extern SymbolTableTree symbol_table_tree;
extern std::map<std::string, SpaceNeeded> map_space_needed_for_function;

class FuncDefAST : public BaseAST
{
public:
    std::unique_ptr<BaseTypeAST> func_type;
    std::string ident;
    std::unique_ptr<BaseAST> block;
    std::unique_ptr<BaseAST> func_f_params;
    void Dump(int ctl = 0) override;
};

#endif