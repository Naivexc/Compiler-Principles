#ifndef CONST_DECL_H
#define CONST_DECL_H

#include "Base.h"
#include <vector>
#include <cassert>

extern bool is_calculating_const_exp;

class ConstDeclAST : public BaseAST
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::vector<std::unique_ptr<BaseAST>> vec_const_def;
    void Dump(int ctl = 0) override;
};

#endif