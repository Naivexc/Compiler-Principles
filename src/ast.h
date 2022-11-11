/**
 * ast.h引用了AST所有节点的类型
 **/
#ifndef AST_H
#define AST_H
#include <iostream>
#include <memory>
#include <stack>
#include <vector>
#include <map>
#include <queue>
#include <assert.h>
#include "InitList.h"
#include "UnaryExp.h"
#include "PrimaryExp.h"
#include "LAndExp.h"
#include "EqExp.h"
#include "RelExp.h"
#include "AddExp.h"
#include "MulExp.h"
#include "LOrExp.h"
#include "Decl.h"
#include "Stmt.h"
#include "VarDef.h"
#include "CompUnitEle.h"
#include "ConstInitVal.h"
#include "FuncFParam.h"
#include "LVal.h"
#include "InitVal.h"
#include "ConstDef.h"
#include "Debug.h"
#include "SymbolTable.h"
#include "CompUnit.h"
#include "FuncDef.h"
#include "FuncType.h"
#include "FuncFParams.h"
#include "Exp.h"
#include "BType.h"
#include "Block.h"
#include "FuncRParams.h"
#include "Number.h"
#include "UnaryOp.h"
#include "UnaryExp.h"
#include "MulOp.h"
#include "AddOp.h"
#include "RelOp.h"
#include "VarDecl.h"
#include "ConstExp.h"
#include "ConstDecl.h"
#include "BlockItem.h"
#include "EqOp.h"
#include "LOrOp.h"
#include "LAndOp.h"
extern std::string itostr(int num);
extern int strtoi(std::string str);
extern std::string AST_name[];
extern bool is_calculating_const_exp;
extern bool last_ins_is_ret;
extern bool str_is_same(const char *s1, const char *s2);
extern int unused_koopa_var_count; //@var_name %d
extern int unused_koopa_label_count;
class CompUnitAST;
class LValAST;
class FuncTypeAST;
class FuncFParamAST;
class ExpAST;
class SymbolTableTree;
class SymbolTable;
class FuncRParamsAST;
struct InitList;
extern std::queue<std::string> global_var_name_queue;
extern void print_dump(std::string AST_name, bool is_debug_begin);
extern void print_ident(int ident);
extern SymbolTableTree symbol_table_tree;
extern SymbolTable *cur_symbol_table;
struct SpaceNeeded;
extern std::map<std::string, SpaceNeeded> map_space_needed_for_function;
extern std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
extern int return_type_space(std::string type);
extern int max_int(int x, int y);

#endif