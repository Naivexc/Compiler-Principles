#ifndef UNARY_EXP_H
#define UNARY_EXP_H
#include "Debug.h"
#include "Base.h"
#include "SymbolTable.h"
#include "SpaceNeeded.h"
#include <memory>

extern SymbolTableTree symbol_table_tree;
extern int32_t unused_koopa_count;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern std::string k_str;

extern int32_t return_type_space(std::string type);
extern std::string itostr(int num);

struct UnaryExpCase0
{
    std::unique_ptr<BaseExpAST> primary_exp;
};

struct UnaryExpCase1
{
    std::string ident;
    std::unique_ptr<BaseAST> func_r_params;
};

struct UnaryExpCase2
{
    std::unique_ptr<BaseOpAST> unary_op;
    std::unique_ptr<BaseExpAST> unary_exp;
};

class UnaryExpAST : public BaseExpAST
{
public:
    // tag 0
    union
    {
        UnaryExpCase0 *unary_exp_case_0;
        UnaryExpCase1 *unary_exp_case_1;
        UnaryExpCase2 *unary_exp_case_2;
    } unary_exp_union;
    void Dump(int ctl = 0) override;
    UnaryExpAST(int __tag__);
};
#endif