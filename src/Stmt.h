#ifndef STMT_H
#define STMT_H
#include "Base.h"
#include "SpaceNeeded.h"
#include "SymbolTable.h"
#include <memory>
#include <map>

extern SymbolTable *cur_symbol_table;
extern SymbolTableTree symbol_table_tree;
extern std::string itostr(int num);
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern int32_t return_type_space(std::string type);
extern int32_t unused_koopa_label_count;
extern bool last_ins_is_ret;

class StmtCase0 // lval=exp;
{
public:
    std::unique_ptr<BaseAST> l_val;
    std::unique_ptr<BaseExpAST> exp;
};

class StmtCase1 //;
{
public:
};

class StmtCase2 // exp;
{
public:
    std::unique_ptr<BaseExpAST> exp;
};

class StmtCase3 // block
{
public:
    std::unique_ptr<BaseAST> block;
};

class StmtCase4 // if
{
public:
    std::unique_ptr<BaseExpAST> exp;
    std::unique_ptr<BaseAST> stmt;
    std::string label_then;
    std::string label_end;
};

class StmtCase5 // if else
{
public:
    std::unique_ptr<BaseExpAST> exp;
    std::unique_ptr<BaseAST> stmt_if;
    std::unique_ptr<BaseAST> stmt_else;
    std::string label_then;
    std::string label_else;
    std::string label_end;
};

class StmtCase6 // while
{
public:
    std::unique_ptr<BaseExpAST> exp;
    std::unique_ptr<BaseAST> stmt;
    std::string label_while;
    std::string label_while_true;
    std::string label_while_false;
};

class StmtCase7 // break
{
public:
    BaseAST *last_while_ins;
};

class StmtCase8 // continue
{
public:
    BaseAST *last_while_ins;
};

class StmtCase9
{
public:
    std::unique_ptr<BaseExpAST> exp;
};

class StmtCase10
{
public:
    // empty
};

class StmtAST : public BaseAST
{
public:
    union
    {
        StmtCase0 *stmt_case_0;
        StmtCase1 *stmt_case_1;
        StmtCase2 *stmt_case_2;
        StmtCase3 *stmt_case_3;
        StmtCase4 *stmt_case_4;
        StmtCase5 *stmt_case_5;
        StmtCase6 *stmt_case_6;
        StmtCase7 *stmt_case_7; // empty
        StmtCase8 *stmt_case_8; // empty
        StmtCase9 *stmt_case_9;
        StmtCase10 *stmt_case_10;
    } stmt_union;
    void Dump(int ctl = 0) override;
    StmtAST(int __tag__);
};

#endif