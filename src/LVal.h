#ifndef L_VAL_H
#define L_VAL_H

#include <string>
#include "CalData.h"
#include <vector>
#include <memory>
#include "Base.h"
extern std::string k_str;
extern int32_t unused_koopa_count;

class LValCase0
{
public:
    std::string ident;
};

class LValCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_exp;
};

class LValAST : public BaseAST
{
public:
    union
    {
        LValCase0 *l_val_case_0;
        LValCase1 *l_val_case_1;
    } l_val_union;
    CalData data;
    void Dump(int ctl = 0) override;
    LValAST(int __tag__);
};
#endif