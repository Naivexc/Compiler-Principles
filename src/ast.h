/*
	ast.h引用了AST所有节点的类型
*/

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
#define IS_CONST true
#define IS_NOT_CONST false
#define STRONG 1
#define WEAK 0
#define GLOBAL 2
#define UN_DEF -1
#define GET_ARRAY_PTR 2
#define NORMAL 0
#define TAG_0 0
#define TAG_1 1
#define TAG_2 2
#define TAG_3 3
#define TAG_4 4
#define TAG_5 5
#define TAG_6 6
#define TAG_7 7
#define TAG_8 8
#define TAG_9 9
#define TAG_10 10
#define DIM_0 0
#define LINE(x)                 \
	{                           \
		printf("line:%d\n", x); \
	}
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

struct SpaceNeeded
{
	int total_needed;
	int return_address_needed;
	int params_needed;
	int variate_needed;
	SpaceNeeded() : total_needed(0), return_address_needed(0), params_needed(0), variate_needed(0){};
};

#pragma endregion FuncFParams

#pragma region FuncType

class FuncTypeAST : public BaseTypeAST
{
public:
	FuncTypeAST(std::string __type__) { type = __type__; };
	FuncTypeAST(){};
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("FuncType", DEBUG_BEGIN);
		if (type == "int")
			k_str += ": i32\n";
		else if (type == "void")
			k_str += '\n';
		PRINT_DUMP("FuncType", DEBUG_END);
	}
};

#pragma endregion FuncType

#pragma region FuncDef

class FuncDefAST : public BaseAST
{
public:
	std::unique_ptr<BaseTypeAST> func_type;
	std::string ident;
	std::unique_ptr<BaseAST> block;
	std::unique_ptr<BaseAST> func_f_params;
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("FuncDef", DEBUG_BEGIN);
		last_ins_is_ret = false;
		auto temp_symbol_table = cur_symbol_table;
		cur_symbol_table->insert(ident, Function(dynamic_cast<FuncTypeAST *>(func_type.get())->type, dynamic_cast<FuncFParamsAST *>(func_f_params.get())->func_f_params));
		cur_symbol_table = symbol_table_tree.symbol_table_tree_root->AddSymbolTable();
		cur_map_iter_for_func_space_needed = map_space_needed_for_function.insert(make_pair(ident, SpaceNeeded())).first;
		k_str += "fun ";
		k_str += ident;
		k_str += '(';
		func_f_params->Dump();
		k_str += ")";
		++depth_dump;
		func_type->Dump();
		--depth_dump;
		k_str += "{\n";
		k_str += "  %entry:\n";
		dynamic_cast<FuncFParamsAST *>(func_f_params.get())->declare_alloc();
		++depth_dump;
		block->Dump();
		if (last_ins_is_ret == false)
		{
			if (dynamic_cast<FuncTypeAST *>(func_type.get())->type == "void")
				k_str += "    ret\n";
			else
				k_str += "    ret 0\n";
		}
		--depth_dump;
		k_str += "}\n";
		cur_symbol_table = temp_symbol_table;
		int S = cur_map_iter_for_func_space_needed->second.variate_needed;
		int R = cur_map_iter_for_func_space_needed->second.return_address_needed;
		int A = cur_map_iter_for_func_space_needed->second.params_needed;
		cur_map_iter_for_func_space_needed->second.total_needed = (S + R + A + 15) / 16;
		cur_map_iter_for_func_space_needed->second.total_needed *= 16;
		PRINT_DUMP("FuncDef", DEBUG_END);
	}
};

#pragma endregion FuncDef

#pragma region Block

class BlockAST : public BaseAST
{
public:
	std::vector<std::unique_ptr<BaseAST>> vec_block_item;
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("Block", DEBUG_BEGIN);
		for (auto iter = vec_block_item.begin(); iter != vec_block_item.end(); ++iter)
		{
			++depth_dump;
			(*iter)->Dump();
			--depth_dump;
		}
		PRINT_DUMP("Block", DEBUG_END);
	}
};

#pragma endregion Block

#pragma region Stmt

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
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("Stmt", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{

			if (stmt_union.stmt_case_0->l_val->tag == 0)
			{
				auto l_val = reinterpret_cast<LValAST *>(stmt_union.stmt_case_0->l_val.get());
				auto iter = cur_symbol_table->find(l_val->l_val_union.l_val_case_0->ident);
				if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end())
				{
					iter = cur_symbol_table->find(std::string("%") + (l_val->l_val_union.l_val_case_0->ident.c_str() + 1));
					if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
						assert(false);
				}
				if (iter->second->tag != 0 || iter->second->data.var->flag_is_const == IS_CONST)
					assert(false);
				++depth_dump;
				stmt_union.stmt_case_0->exp->Dump();
				--depth_dump;
				k_str += ("    store ");
				if (stmt_union.stmt_case_0->exp->data.is_num == true)
					k_str += itostr(stmt_union.stmt_case_0->exp->data.val);
				else
				{
					k_str += '%';
					k_str += itostr(stmt_union.stmt_case_0->exp->data.count);
				}
				k_str += (", " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + '\n');
				if (iter->first == "%l")
					assert(false);
			}
			else if (stmt_union.stmt_case_0->l_val->tag == 1)
			{
				auto l_val = reinterpret_cast<LValAST *>(stmt_union.stmt_case_0->l_val.get());
				auto iter = cur_symbol_table->find(l_val->l_val_union.l_val_case_1->ident);
				for (int i = 0; i < l_val->l_val_union.l_val_case_1->vec_exp.size(); ++i)
					l_val->l_val_union.l_val_case_1->vec_exp[i]->Dump();
				if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end())
				{
					iter = cur_symbol_table->find(std::string("%") + (l_val->l_val_union.l_val_case_0->ident.c_str() + 1));
					if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
						assert(false);
				}
				if (iter->second->tag != 0)
					assert(false);
				if (iter->second->data.var->var_type.substr(0, 2) == "**")
				{
					k_str += ("    %" + itostr(unused_koopa_count) + " = load " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + '\n');
					++unused_koopa_count;
					cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				}
				if (iter->second->data.var->var_type.substr(0, 2) == "*[")
					k_str += "    %" + itostr(unused_koopa_count) + " = getelemptr " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + ", ";
				else if (iter->second->data.var->var_type.substr(0, 2) == "**")
					k_str += "    %" + itostr(unused_koopa_count) + " = getptr %" + itostr(unused_koopa_count - 1) + ", "; // naive
				else
				{
					printf("%s\n", iter->second->data.var->var_type.c_str());
					assert(false);
				}
				l_val->l_val_union.l_val_case_1->vec_exp[0]->data.print_data_to_k_str();
				k_str += '\n';
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");

				for (int i = 1; i < l_val->l_val_union.l_val_case_1->vec_exp.size(); ++i)
				{
					k_str += "    %" + itostr(unused_koopa_count) + " = getelemptr %" + itostr(unused_koopa_count - 1) + ", ";
					l_val->l_val_union.l_val_case_1->vec_exp[i]->data.print_data_to_k_str();
					k_str += '\n';
					++unused_koopa_count;
					cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				}
				int num_dst = unused_koopa_count - 1;
				stmt_union.stmt_case_0->exp->Dump();
				k_str += ("    store ");
				stmt_union.stmt_case_0->exp->data.print_data_to_k_str();
				k_str += (", %" + itostr(num_dst) + '\n');
				break;
			}
			else
				assert(false);
			break;
		}
		case (1):
		{
			/*empty*/
			break;
		}
		case (2):
		{
			stmt_union.stmt_case_2->exp->Dump();
			break;
		}
		case (3):
		{
			SymbolTable *temp_cur_symbol_table = cur_symbol_table;
			cur_symbol_table = cur_symbol_table->AddSymbolTable();
			stmt_union.stmt_case_3->block->Dump();
			cur_symbol_table = temp_cur_symbol_table;
			break;
		}
		case (4):
		{
			stmt_union.stmt_case_4->exp->Dump();
			stmt_union.stmt_case_4->label_then = "%then_" + itostr(unused_koopa_label_count);
			++unused_koopa_label_count;
			stmt_union.stmt_case_4->label_end = "%end_" + itostr(unused_koopa_label_count);
			++unused_koopa_label_count;
			std::string label_then = stmt_union.stmt_case_4->label_then;
			std::string label_end = stmt_union.stmt_case_4->label_end;
			if (stmt_union.stmt_case_4->exp->data.is_num == false)
			{
				k_str += "    br %";
				k_str += itostr(stmt_union.stmt_case_4->exp->data.count);
			}
			else
			{
				k_str += "    br ";
				k_str += itostr(stmt_union.stmt_case_4->exp->data.val);
			}
			k_str += (", " + stmt_union.stmt_case_4->label_then + ", " + label_end + '\n');
			k_str += ("  " + label_then + ':');
			k_str += '\n';
			stmt_union.stmt_case_4->stmt->Dump();
			if (last_ins_is_ret == false)
				k_str += ("    jump " + label_end + '\n');
			else
				last_ins_is_ret = false;
			k_str += ("  " + label_end + ':');
			k_str += '\n';
			break;
		}
		case (5):
		{
			stmt_union.stmt_case_5->exp->Dump();
			stmt_union.stmt_case_5->label_then = "%then_" + itostr(unused_koopa_label_count);
			++unused_koopa_label_count;
			stmt_union.stmt_case_5->label_else = "%else_" + itostr(unused_koopa_label_count);
			++unused_koopa_label_count;
			stmt_union.stmt_case_5->label_end = "%end_" + itostr(unused_koopa_label_count);
			++unused_koopa_label_count;
			std::string label_then = stmt_union.stmt_case_5->label_then;
			std::string label_else = stmt_union.stmt_case_5->label_else;
			std::string label_end = stmt_union.stmt_case_5->label_end;
			if (stmt_union.stmt_case_5->exp->data.is_num == false)
			{
				k_str += "    br %";
				k_str += itostr(stmt_union.stmt_case_5->exp->data.count);
			}
			else
			{
				k_str += "    br ";
				k_str += itostr(stmt_union.stmt_case_5->exp->data.val);
			}
			k_str += (", " + label_then + ", " + label_else + '\n' + '\n');
			k_str += ("  " + label_then + ":\n");
			stmt_union.stmt_case_5->stmt_if->Dump();
			if (last_ins_is_ret == false)
				k_str += ("    jump " + label_end + '\n');
			else
				last_ins_is_ret = false;
			k_str += '\n';
			k_str += ("  " + label_else + ":\n");
			stmt_union.stmt_case_5->stmt_else->Dump();
			if (last_ins_is_ret == false)
				k_str += ("    jump " + label_end + '\n');
			else
				last_ins_is_ret = false;
			k_str += '\n';
			k_str += ("  " + label_end + ':');
			k_str += '\n';
			break;
		}
		case (6):
		{

			stmt_union.stmt_case_6->label_while = "%while_" + itostr(unused_koopa_label_count);
			std::string label_while = stmt_union.stmt_case_6->label_while;
			++unused_koopa_label_count;
			stmt_union.stmt_case_6->label_while_true = "%while_true_" + itostr(unused_koopa_label_count);
			std::string label_while_true = stmt_union.stmt_case_6->label_while_true;
			++unused_koopa_label_count;
			stmt_union.stmt_case_6->label_while_false = "%while_false_" + itostr(unused_koopa_label_count);
			std::string label_while_false = stmt_union.stmt_case_6->label_while_false;
			++unused_koopa_label_count;
			if (last_ins_is_ret == false)
				k_str += ("    jump " + label_while + '\n');
			else
				last_ins_is_ret = false;
			k_str += '\n';
			k_str += ("  " + label_while + ':' + '\n');
			stmt_union.stmt_case_6->exp->Dump();
			if (stmt_union.stmt_case_6->exp->data.is_num == false)
			{
				k_str += "    br %";
				k_str += itostr(stmt_union.stmt_case_6->exp->data.count);
			}
			else if (stmt_union.stmt_case_6->exp->data.val == 0)
			{
				// while???,
				// k_str += "    br ";
				// k_str += itostr(stmt_union.stmt_case_6->exp->data.val);
				k_str += ("    jump " + label_while_false + '\n');
				k_str += '\n';
				k_str += ("  " + label_while_false + ':');
				k_str += '\n';
				return;
			}
			else
			{
				k_str += "    br ";
				k_str += itostr(stmt_union.stmt_case_6->exp->data.val);
			}
			k_str += (", " + label_while_true + ", " + label_while_false + '\n');
			k_str += '\n';
			k_str += ("  " + label_while_true + ':');
			k_str += '\n';
			stmt_union.stmt_case_6->stmt->Dump();
			if (last_ins_is_ret == false)
				k_str += ("    jump " + label_while + '\n');
			else
				last_ins_is_ret = false;
			k_str += '\n';
			k_str += ("  " + label_while_false + ':');
			k_str += '\n';
			break;
		}
		case (7):
		{
			BaseAST *last_while_ins = stmt_union.stmt_case_7->last_while_ins;
			std::string label_while_never_access = ("%while_never_access_" + itostr(unused_koopa_label_count));
			++unused_koopa_label_count;
			k_str += ("    jump " + dynamic_cast<StmtAST *>(last_while_ins)->stmt_union.stmt_case_6->label_while_false + '\n');
			k_str += '\n';
			k_str += ("  " + label_while_never_access + ':');
			k_str += '\n';
			break;
		}
		case (8):
		{
			BaseAST *last_while_ins = stmt_union.stmt_case_8->last_while_ins;
			std::string label_while_never_access = ("%while_never_access_" + itostr(unused_koopa_label_count));
			++unused_koopa_label_count;
			k_str += ("    jump " + dynamic_cast<StmtAST *>(last_while_ins)->stmt_union.stmt_case_6->label_while + '\n');
			k_str += '\n';
			k_str += ("  " + label_while_never_access + ':');
			k_str += '\n';
			break;
		}
		case (9):
		{
			++depth_dump;
			stmt_union.stmt_case_9->exp->Dump();
			--depth_dump;
			k_str += "    ret ";
			if (stmt_union.stmt_case_9->exp->data.is_num == true)
				k_str += itostr(stmt_union.stmt_case_9->exp->data.val);
			else
			{
				k_str += '%';
				k_str += itostr(stmt_union.stmt_case_9->exp->data.count);
			}
			k_str += '\n';
			last_ins_is_ret = true;
			break;
		}
		case (10):
		{
			k_str += "    ret\n";
			last_ins_is_ret = true;
			break;
		}
		default:
		{
			assert(false);
			break;
		}
		}
		PRINT_DUMP("Stmt", DEBUG_END);
	}
	StmtAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			stmt_union.stmt_case_0 = new StmtCase0();
			break;
		case 1:
			stmt_union.stmt_case_1 = new StmtCase1();
			break;
		case 2:
			stmt_union.stmt_case_2 = new StmtCase2();
			break;
		case 3:
			stmt_union.stmt_case_3 = new StmtCase3();
			break;
		case 4:
			stmt_union.stmt_case_4 = new StmtCase4();
			break;
		case 5:
			stmt_union.stmt_case_5 = new StmtCase5();
			break;
		case 6:
			stmt_union.stmt_case_6 = new StmtCase6();
			break;
		case 7:
			stmt_union.stmt_case_7 = new StmtCase7();
			break;
		case 8:
			stmt_union.stmt_case_8 = new StmtCase8();
			break;
		case 9:
			stmt_union.stmt_case_9 = new StmtCase9();
			break;
		case 10:
			stmt_union.stmt_case_10 = new StmtCase10();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Stmt

#pragma region FuncRParams

class FuncRParamsAST : public BaseAST
{
public:
	std::vector<ExpAST *> func_r_params;
	void Dump(int ctl = 0) override
	{
		if (ctl == 0)
		{
			if (func_r_params.empty())
				return;
			std::vector<ExpAST *>::iterator iter = func_r_params.begin();
			(*iter)->data.print_data_to_k_str();
			++iter;
			while (iter != func_r_params.end())
			{
				k_str += ',';
				(*iter)->data.print_data_to_k_str();
				++iter;
			}
		}
		else
		{
			cur_map_iter_for_func_space_needed->second.params_needed = max_int(cur_map_iter_for_func_space_needed->second.params_needed, (func_r_params.size() - 8) * 4);
			if (func_r_params.empty())
				return;
			std::vector<ExpAST *>::iterator iter = func_r_params.begin();
			while (iter != func_r_params.end())
			{
				(*iter)->Dump();
				++iter;
			}
		}
	}
};

#pragma endregion FuncRParams

#pragma region InitVal

class InitValAST : public BaseAST
{
public:
	union
	{
		InitValCase0 *init_val_case_0;
		InitValCase1 *init_val_case_1;
	} init_val_union;
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("InitVal", DEBUG_BEGIN);
		++depth_dump;
		switch (tag)
		{
		case (0):
		{
			init_val_union.init_val_case_0->exp->Dump();
			init_val_union.init_val_case_0->data = init_val_union.init_val_case_0->exp->data;
			break;
		}
		case (1):
		{
			for (int i = 0; i < init_val_union.init_val_case_1->vec_init_val.size(); ++i)
			{
				++depth_dump;
				init_val_union.init_val_case_1->vec_init_val[i]->Dump();
				--depth_dump;
			}
			break;
		}
		default:
			assert(false);
			break;
		}
		--depth_dump;
		PRINT_DUMP("InitVal", DEBUG_END);
	}
	InitValAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			init_val_union.init_val_case_0 = new InitValCase0();
			break;
		case 1:
			init_val_union.init_val_case_1 = new InitValCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

class BlockItemAST : public BaseAST
{
public:
	// tag 0
	std::unique_ptr<BaseAST> decl;
	// tag 1
	std::unique_ptr<BaseAST> stmt;
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("BlockItem", DEBUG_BEGIN);
		if (last_ins_is_ret == true)
		{
			k_str += ("  %block_" + itostr(unused_koopa_label_count) + ":\n");
			++unused_koopa_label_count;
			last_ins_is_ret = false;
		}
		++depth_dump;
		if (tag == 0)
			decl->Dump();
		else
			stmt->Dump();
		--depth_dump;
		PRINT_DUMP("BlockItem", DEBUG_END);
	}
};

#pragma endregion BlockItem

#pragma region VarDecl

class VarDeclAST : public BaseAST
{
public:
	std::unique_ptr<BaseTypeAST> b_type;
	std::vector<std::unique_ptr<BaseAST>> vec_var_def;
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("VarDecl", DEBUG_BEGIN);
		++depth_dump;
		b_type->Dump();
		--depth_dump;
		++depth_dump;
		for (auto iter = vec_var_def.begin(); iter != vec_var_def.end(); ++iter)
		{
			(*iter)->Dump();
		}
		--depth_dump;
		PRINT_DUMP("VarDecl", DEBUG_END);
	}
};

#pragma endregion VarDecl

#pragma region VarDef

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
	VarDefAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			var_def_union.var_def_case_0 = new VarDefCase0();
			break;
		case 1:
			var_def_union.var_def_case_1 = new VarDefCase1();
			break;
		case 2:
			var_def_union.var_def_case_2 = new VarDefCase2();
			break;
		default:
			assert(false);
		}
		return;
	}
	void InitGlobalArrayInKoopa(int dim)
	{
		switch (tag)
		{
		case (0):
			// tag==0??????,???????koopa?��????
			break;
		case (1):
		{
			if (dim == var_def_union.var_def_case_1->vec_const_exp.size())
			{
				if (cur_init_ele_koopa_list->tag == 1)
					assert(false);
				cur_init_ele_koopa_list->init_list_union.init_list_case_0->data.print_data_to_k_str();
				return;
			}
			int vec_const_exp_dim_len = var_def_union.var_def_case_1->vec_const_exp[dim]->data.val;
			auto temp_init_list = cur_init_ele_koopa_list;
			for (int i = 0; i < vec_const_exp_dim_len; ++i)
			{
				if (temp_init_list->tag == 0)
					assert(false);
				cur_init_ele_koopa_list = temp_init_list->init_list_union.init_list_case_1->vec_init_list[i].get();
				if (cur_init_ele_koopa_list->tag == 1)
					k_str += '{';
				InitGlobalArrayInKoopa(dim + 1);
				cur_init_ele_koopa_list = temp_init_list->init_list_union.init_list_case_1->vec_init_list[i].get();
				if (cur_init_ele_koopa_list->tag == 1)
					k_str += '}';
				if (i != vec_const_exp_dim_len - 1)
					k_str += ", ";
			}
			break;
		}
		default:
			assert(false);
		}
	}
	void InitLocalArrayInKoopa(int dim, std::string str_src)
	{
		switch (tag)
		{
		case (0):
			// tag==0??????,???????koopa?��????
			break;
		case (1):
		{
			if (dim == var_def_union.var_def_case_1->vec_const_exp.size())
			{
				if (cur_init_ele_koopa_list->tag == 1)
					assert(false);
				k_str += "    store ";
				cur_init_ele_koopa_list->init_list_union.init_list_case_0->data.print_data_to_k_str();
				k_str += (", " + str_src + '\n');
				return;
			}
			int vec_const_exp_dim_len = var_def_union.var_def_case_1->vec_const_exp[dim]->data.val;
			auto temp_init_list = cur_init_ele_koopa_list;
			for (int i = 0; i < vec_const_exp_dim_len; ++i)
			{
				if (temp_init_list->tag == 0)
					assert(false);
				cur_init_ele_koopa_list = temp_init_list->init_list_union.init_list_case_1->vec_init_list[i].get();
				k_str += ("    %" + itostr(unused_koopa_count) + " = getelemptr " + str_src + ", " + itostr(i) + '\n');
				std::string str_dst = ("%" + itostr(unused_koopa_count));
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				InitLocalArrayInKoopa(dim + 1, str_dst);
			}
			break;
		}
		default:
			assert(false);
		}
	}
	void Dump(int ctl = 0) override
	{
		PRINT_DUMP("ConstDef", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			size = 4;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
			{
				cur_symbol_table->insert(var_def_union.var_def_case_0->ident, Variate(IS_NOT_CONST, UN_DEF, "*i32", WEAK));
				k_str += ("    " + var_def_union.var_def_case_0->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + "i32\n");
				++unused_koopa_var_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += size;
			}
			else
			{
				cur_symbol_table->insert(var_def_union.var_def_case_0->ident, Variate(IS_NOT_CONST, UN_DEF, "*i32", GLOBAL));
				// global @var = alloc i32, zeroinit
				k_str += ("global " + var_def_union.var_def_case_0->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + "i32" + ", zeroinit\n");
				++unused_koopa_var_count;
				global_var_name_queue.push(var_def_union.var_def_case_0->ident);
			}
			break;
		}
		case (2):
		{
			size = 4;
			++depth_dump;
			var_def_union.var_def_case_2->init_val->Dump();
			if (var_def_union.var_def_case_2->init_val->tag != 0)
				assert(false);
			data = dynamic_cast<InitValAST *>(var_def_union.var_def_case_2->init_val.get())->init_val_union.init_val_case_0->data;
			--depth_dump;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
			{
				cur_symbol_table->insert(var_def_union.var_def_case_2->ident, Variate(IS_NOT_CONST, data.val, "*i32", STRONG));
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				k_str += ("    " + var_def_union.var_def_case_2->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + "i32\n");
				if (data.is_num == true)
					k_str += (std::string("    store ") + itostr(data.val) + std::string(", ") + var_def_union.var_def_case_2->ident + '_' + itostr(unused_koopa_var_count) + '\n');
				else
				{
					k_str += (std::string("    store ") + std::string("%") + itostr(data.count) + std::string(", ") + var_def_union.var_def_case_2->ident + '_' + itostr(unused_koopa_var_count) + '\n');
				}
				++unused_koopa_var_count;
			}
			else
			{
				cur_symbol_table->insert(var_def_union.var_def_case_2->ident, Variate(IS_NOT_CONST, data.val, "*i32", GLOBAL));
				k_str += ("global " + var_def_union.var_def_case_2->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + "i32" + ", ");
				if (data.is_num == true)
					k_str += (itostr(data.val) + '\n');
				else
				{
					k_str += (std::string("%") + itostr(data.count) + '\n');
				}
				++unused_koopa_var_count;
				global_var_name_queue.push(var_def_union.var_def_case_2->ident);
			}
			break;
			break;
		}
		case (1):
		{
			size = 4;
			for (int i = 0; i < var_def_union.var_def_case_1->vec_const_exp.size(); ++i)
			{
				is_calculating_const_exp = true;
				var_def_union.var_def_case_1->vec_const_exp[i]->Dump();
				size *= var_def_union.var_def_case_1->vec_const_exp[i]->data.val;
				is_calculating_const_exp = false;
			}
			var_def_union.var_def_case_1->init_val->Dump();
			Init();
			Convert(dynamic_cast<InitValAST *>(var_def_union.var_def_case_1->init_val.get()));
			cur_init_ele_koopa_list = init_list.get();
			std::string arr_type = var_def_union.var_def_case_1->return_type();
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get()) // not global
			{
				auto pair_ident = cur_symbol_table->insert(var_def_union.var_def_case_1->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + arr_type, STRONG));
				std::string str_src = var_def_union.var_def_case_1->ident + '_' + itostr(pair_ident.first->second->data.var->koopa_var_count);
				k_str += ("    " + str_src + " = alloc " + arr_type + "\n");
				++unused_koopa_var_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += size;
				InitLocalArrayInKoopa(DIM_0, str_src);
			}
			else
			{
				cur_symbol_table->insert(var_def_union.var_def_case_1->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + arr_type, GLOBAL));
				k_str += ("global " + var_def_union.var_def_case_1->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + arr_type + ", {");
				global_var_name_queue.push(var_def_union.var_def_case_1->ident);
				++unused_koopa_var_count;
				InitGlobalArrayInKoopa(DIM_0);
				k_str += "}\n";
			}
			break;
		}
		default:
			assert(false);
			break;
		}
		PRINT_DUMP("ConstDef", DEBUG_END);
	}
	void Init()
	{

		if (tag == 0 || tag == 2) // not array
		{
			// const_init_val_stantard = unique_ptr<ConstInitValAST>(new ConstInitValAST(TAG_0));
		}
		else if (tag == 1)
		{
			auto iter_begin = var_def_union.var_def_case_1->vec_const_exp.begin();
			auto iter_end = var_def_union.var_def_case_1->vec_const_exp.end();
			init_list = std::unique_ptr<InitList>(new InitList(TAG_1));
			ConstInitValInit(iter_begin, iter_end, init_list.get()); //?????????const??
			std::vector<std::unique_ptr<InitList>>::iterator temp = init_list->init_list_union.init_list_case_1->vec_init_list.begin();
			vec_begin_init_list.push_back(temp);
			vec_cur_init_list.push_back(temp);
			vec_end_init_list.push_back(init_list->init_list_union.init_list_case_1->vec_init_list.end());
			while ((*temp)->tag != 0)
			{
				vec_end_init_list.push_back((*temp)->init_list_union.init_list_case_1->vec_init_list.end());
				temp = (*temp)->init_list_union.init_list_case_1->vec_init_list.begin();
				vec_begin_init_list.push_back(temp);
				vec_cur_init_list.push_back(temp);
			}
		}
		else
			assert(false);
	}
	int cur_init_index()
	{
		int index = vec_cur_init_list.size() - 1;
		while (index >= 0 && vec_cur_init_list[index] == vec_begin_init_list[index])
			--index;
		if (index < 0)
			++index;
		return index;
	}
	void Convert(InitValAST *iv)
	{
		if (iv->tag == 0)
		{
			auto iter = vec_cur_init_list.end() - 1;
			// iter????????initlist,tag?0
			if ((*(*iter))->tag != 0)
				assert(false);
			(*(*iter))->init_list_union.init_list_case_0->data = iv->init_val_union.init_val_case_0->data;
			++(vec_cur_init_list.back());
			int index = vec_cur_init_list.size() - 1; // index???????????end()??iter??vector?��?��??
			while (vec_cur_init_list[index] == vec_end_init_list[index])
			{
				--index;
				if (index < 0)
				{

					for (int i = 0; i < vec_begin_init_list.size(); ++i)
					{
						if (vec_cur_init_list[i] != vec_end_init_list[i])
							assert(false);
					}
					return;
				}
				++(vec_cur_init_list[index]);
			}
			//????????vec
			for (int i = index + 1; i < vec_cur_init_list.size(); ++i)
			{
				vec_cur_init_list[i] = (*(vec_cur_init_list[i - 1]))->init_list_union.init_list_case_1->vec_init_list.begin();
				vec_begin_init_list[i] = vec_cur_init_list[i];
				vec_end_init_list[i] = (*(vec_cur_init_list[i - 1]))->init_list_union.init_list_case_1->vec_init_list.end();
			}
		}
		else
		{
			int index = cur_init_index();
			auto iter_index = vec_cur_init_list[index];
			int iv_size = iv->init_val_union.init_val_case_1->vec_init_val.size();
			for (int i = 0; i < iv_size; ++i)
			{
				Convert(dynamic_cast<InitValAST *>(iv->init_val_union.init_val_case_1->vec_init_val[i].get()));
			}
			while (iter_index == vec_cur_init_list[index])
			{
				Convert(&zero_init);
			}
			while (vec_cur_init_list[index] == vec_end_init_list[index])
			{
				--index;
				if (index < 0)
				{
					for (int i = 0; i < vec_begin_init_list.size(); ++i)
					{
						if (vec_cur_init_list[i] != vec_end_init_list[i])
							assert(false);
					}
					return;
				}
				++(vec_cur_init_list[index]);
			}
			//????????vec
			for (int i = index + 1; i < vec_cur_init_list.size(); ++i)
			{
				vec_cur_init_list[i] = (*(vec_cur_init_list[i - 1]))->init_list_union.init_list_case_1->vec_init_list.begin();
				vec_begin_init_list[i] = vec_cur_init_list[i];
				vec_end_init_list[i] = (*(vec_cur_init_list[i - 1]))->init_list_union.init_list_case_1->vec_init_list.end();
			}
		}
	}
};
#endif