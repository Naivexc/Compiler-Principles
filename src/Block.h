#include "Base.h"

class BlockAST : public BaseAST
{
public:
    std::vector<std::unique_ptr<BaseAST>> vec_block_item;
    void Dump(int ctl = 0) override;
};