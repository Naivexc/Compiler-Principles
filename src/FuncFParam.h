#ifndef FUNC_F_PARAM_H
#define FUNC_F_PARAM_H

#include "Base.h"
#include "Variate.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <memory>
#include <vector>
#include <map>

class SymbolTable;

extern std::string k_str;
extern bool is_calculating_const_exp;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern SymbolTable *cur_symbol_table;

extern std::string itostr(int num);

class FuncFParamCase0
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::string ident;
};

class FuncFParamCase1
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_const_exp;
    std::string return_type();
};

class FuncFParamAST : public BaseAST
{
public:
    union
    {
        FuncFParamCase0 *func_f_param_case_0;
        FuncFParamCase1 *func_f_param_case_1;
    } func_f_param_union;
    int size;
    std::string koopa_type;
    FuncFParamAST(int __tag__, std::string __ident__ = "");
    void Dump(int ctl = 0) override;
    void declare_alloc();
};

#endif