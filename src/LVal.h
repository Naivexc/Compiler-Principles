#ifndef L_VAL_H
#define L_VAL_H

#include "CalData.h"
#include "SymbolTable.h"
#include "Base.h"
#include "SpaceNeeded.h"
#include <vector>
#include <memory>
#include <string>
#include <cassert>

extern std::string k_str;
extern int32_t unused_koopa_count;
extern SymbolTableTree symbol_table_tree;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;

extern std::string itostr(int num);
extern int32_t return_type_space(std::string type);

class LValCase0
{
public:
    std::string ident;
};

class LValCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_exp;
};

class LValAST : public BaseAST
{
public:
    union
    {
        LValCase0 *l_val_case_0;
        LValCase1 *l_val_case_1;
    } l_val_union;
    CalData data;
    void Dump(int ctl = 0) override;
    LValAST(int __tag__);
};
#endif