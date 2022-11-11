#ifndef CONST_DEF_H
#define CONST_DEF_H

#include "Debug.h"
#include "SymbolTable.h"

class ConstDefCase0
{
public:
    std::string ident;
    std::unique_ptr<BaseAST> const_init_val;
    CalData data;
};

class ConstDefCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_const_exp;
    std::unique_ptr<BaseAST> const_init_val;
    CalData data;
    std::string return_type();
};

class ConstDefAST : public BaseAST
{
public:
    union
    {
        ConstDefCase0 *const_def_case_0;
        ConstDefCase1 *const_def_case_1;
    } const_def_union;
    int size;
    static ConstInitValAST zero_init;
    std::unique_ptr<InitList> init_list;
    InitList *cur_init_ele_koopa_list;
    std::vector<std::vector<std::unique_ptr<InitList>>::iterator> vec_begin_init_list;
    std::vector<std::vector<std::unique_ptr<InitList>>::iterator> vec_cur_init_list;
    std::vector<std::vector<std::unique_ptr<InitList>>::iterator> vec_end_init_list;
    ConstDefAST(int __tag__);
    void InitGlobalArrayInKoopa(int dim);
    void InitLocalArrayInKoopa(int dim, std::string str_src);
    void Dump(int ctl = 0) override;
    void Init();
    int cur_init_index();
    void Convert(ConstInitValAST *civ);
};

#endif