#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "Base.h"
#include <memory>
#include <string>

extern bool last_ins_is_ret;
extern std::string k_str;
extern int32_t unused_koopa_label_count;

extern std::string itostr(int num);

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