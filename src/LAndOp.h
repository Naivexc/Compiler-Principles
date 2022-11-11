#ifndef L_AND_OP_H
#define L_AND_OP_H

#include "Base.h"

extern bool is_calculating_const_exp;
extern std::string k_str;
extern int32_t unused_koopa_count = 0;
extern SymbolTable *cur_symbol_table;

extern std::string itostr(int num);

class LAndOpAST : public BaseOpAST
{
public:
    char op[3];
    void Dump(int ctl = 0) override;
};

#endif