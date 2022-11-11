#ifndef L_OR_OP_H
#define L_OR_OP_H

#include "Base.h"
#include "SymbolTable.h"
#include <map>
#include <string>
#include "SpaceNeeded.h"

extern bool is_calculating_const_exp;
extern std::string k_str;
extern int32_t unused_koopa_count;
extern SymbolTable *cur_symbol_table;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern SymbolTableTree symbol_table_tree;

extern int return_type_space(std::string type);

extern std::string itostr(int num);

class LOrOpAST : public BaseOpAST
{
public:
    char op[3];
    void Dump(int ctl = 0) override;
};

#endif