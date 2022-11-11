#ifndef UNARY_OP_H
#define UNARY_OP_H

#include "Base.h"
#include <cassert>
#include <string>

extern bool is_calculating_const_exp;
extern std::string k_str;
extern int32_t unused_koopa_count = 0;
extern std::string itostr(int num);
extern SymbolTable *cur_symbol_table;

class UnaryOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl = 0);
};

#endif