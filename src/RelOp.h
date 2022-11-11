#ifndef REL_OP_H
#define REL_OP_H

#include "Base.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <map>

extern bool is_calculating_const_exp;
extern std::string k_str;
extern std::string itostr(int num);
extern int32_t unused_koopa_count;
extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern int32_t return_type_space(std::string type);

class RelOpAST : public BaseOpAST
{
public:
    // tag 0:>|<;tag 1??>=|<=
    char op[3];
    void Dump(int ctl = 0) override;
};

#endif