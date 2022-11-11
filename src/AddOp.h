#ifndef ADD_OP_H
#define ADD_OP_H

#include "Base.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <cassert>
#include <string>

extern int32_t unused_koopa_count;
extern SymbolTableTree symbol_table_tree;
extern bool is_calculating_const_exp;
extern std::string k_str;
extern SymbolTable *cur_symbol_table;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;

extern std::string itostr(int num);
extern int32_t return_type_space(std::string type);

class AddOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl = 0) override;
};

#endif