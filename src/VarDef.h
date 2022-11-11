#ifndef VAR_DEF_H
#define VAR_DEF_H

#include "Base.h"
#include "InitList.h"
#include "InitVal.h"
#include "SpaceNeeded.h"
#include "Variate.h"
#include "ConstInitVal.h"
#include "SymbolTable.h"
#include <vector>
#include <memory>
#include <cassert>
#include <string>
#include <map>
#include <queue>

extern std::string k_str;
extern std::string itostr(int32_t num);
extern int32_t unused_koopa_count;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;
extern int32_t unused_koopa_var_count;
extern std::queue<std::string> global_var_name_queue;
extern bool is_calculating_const_exp;

extern int return_type_space(std::string type);

class VarDefCase0
{
public:
    std::string ident;
};

class VarDefCase1
{
public:
    std::string ident;
    std::vector<std::unique_ptr<BaseExpAST>> vec_const_exp;
    std::unique_ptr<BaseAST> init_val;
    std::string return_type();
};

class VarDefCase2
{
public:
    std::string ident;
    std::unique_ptr<BaseAST> init_val;
};

class VarDefAST : public BaseAST
{
public:
    union
    {
        VarDefCase0 *var_def_case_0;
        VarDefCase1 *var_def_case_1;
        VarDefCase2 *var_def_case_2;
    } var_def_union;
    int size;
    CalData data;
    static InitValAST zero_init;
    std::unique_ptr<InitList> init_list;
    InitList *cur_init_ele_koopa_list; //????koopa?��???????????init_list?��????
    std::vector<std::vector<std::unique_ptr<InitList>>::iterator> vec_begin_init_list;
    std::vector<std::vector<std::unique_ptr<InitList>>::iterator> vec_cur_init_list; //??��????????val
    std::vector<std::vector<std::unique_ptr<InitList>>::iterator> vec_end_init_list; //???vec_cur_init_list??iter?????????
    VarDefAST(int __tag__);
    void InitGlobalArrayInKoopa(int dim);
    void InitLocalArrayInKoopa(int dim, std::string str_src);
    void Dump(int ctl = 0) override;
    void Init();
    int cur_init_index();
    void Convert(InitValAST *iv);
};

#endif