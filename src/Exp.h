#ifndef EXP_H
#define EXP_H

#include "Base.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <memory>

extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;
extern bool is_calculating_const_exp;

class ExpAST : public BaseExpAST
{
public:
    std::unique_ptr<BaseExpAST> l_or_exp;
    void Dump(int ctl = 0) override;
};

#endif