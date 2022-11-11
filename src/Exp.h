#ifndef EXP_H
#define EXP_H

#include "Base.h"

class ExpAST : public BaseExpAST
{
public:
    std::unique_ptr<BaseExpAST> l_or_exp;
    void Dump(int ctl = 0) override;
};

#endif