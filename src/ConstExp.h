#ifndef CONST_EXP_H
#define CONST_EXP_H

#include "Base.h"
#include "memory"

class ConstExpAST : public BaseExpAST
{
public:
    std::unique_ptr<BaseExpAST> exp;
    void Dump(int ctl = 0) override;
};

#endif