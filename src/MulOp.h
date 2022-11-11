#ifndef MUL_OP_H
#define MUL_OP_H

#include "Base.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <string>
#include <cassert>
#include <map>

extern std::string k_str;
extern bool is_calculating_const_exp;
extern int32_t unused_koopa_count;
extern std::string itostr(int num);
extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;

extern int32_t return_type_space(std::string type);

class MulOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl = 0) override;
};

#endif