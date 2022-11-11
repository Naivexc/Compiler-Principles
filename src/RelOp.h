#ifndef REL_OP_H
#define REL_OP_H

#include "Base.h"

class RelOpAST : public BaseOpAST
{
public:
    // tag 0:>|<;tag 1??>=|<=
    char op[3];
    void Dump(int ctl = 0) override;
};

#endif