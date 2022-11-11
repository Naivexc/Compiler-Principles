#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "Base.h"

class BlockItemAST : public BaseAST
{
public:
    // tag 0
    std::unique_ptr<BaseAST> decl;
    // tag 1
    std::unique_ptr<BaseAST> stmt;
    void Dump(int ctl = 0) override;
};

#endif