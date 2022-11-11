#ifndef EQ_OP_H
#define EQ_OP_H

#include "Base.h"
#include <string>
#include <string.h>

extern bool is_calculating_const_exp;
extern int32_t unused_koopa_count = 0;
extern std::string k_str;
extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;

extern std::string itostr(int num);

class EqOpAST : public BaseOpAST
{
public:
    char op[3];
    void Dump(int ctl = 0) override;
};

#endif