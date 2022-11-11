#ifndef UNARY_OP_H
#define UNARY_OP_H

#include "Base.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <cassert>
#include <string>
#include <map>

extern bool is_calculating_const_exp;
extern std::string k_str;
extern int32_t unused_koopa_count;
extern std::string itostr(int num);
extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;
extern int32_t return_type_space(std::string type);
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;

class UnaryOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl);
};

#endif