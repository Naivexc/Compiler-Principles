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
#define DEBUG_BEGIN true
#define DEBUG_END false
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
class CalData
{
public:
	bool is_num;
	int val;
	int count;
	void operator=(CalData);
	void print_data_to_k_str();
	CalData();
};

class BaseAST
{
public:
	int tag;
	virtual ~BaseAST() = default;

	virtual void Dump(int ctl = 0) = 0;
};

class BaseExpAST : public BaseAST
{
public:
	CalData data;
	virtual ~BaseExpAST() = default;
	virtual void Dump(int ctl = 0) = 0;
};

class BaseOpAST : public BaseAST
{
public:
	CalData lhs;
	CalData rhs;
	CalData result;
	virtual ~BaseOpAST() = default;
	virtual void Dump(int ctl = 0) = 0;
};

class BaseTypeAST : public BaseAST
{
public:
	std::string type;
	virtual ~BaseTypeAST() = default;
	virtual void Dump(int ctl = 0) = 0;
};

class Variate
{
public:
	bool flag_is_const;
	int val;
	std::string var_type; //"int"
	int sym_type;		  // STRONG WEAK
	int koopa_var_count;
	int dim;
	Variate(bool __is_const__, int __val__, std::string __var_type__, int __sym_type__) : flag_is_const(__is_const__), val(__val__), var_type(__var_type__), sym_type(__sym_type__), koopa_var_count()
	{
		if (__is_const__ == false)
		{
			koopa_var_count = unused_koopa_var_count;
		}
		else
		{
			koopa_var_count = -1;
		}
		dim = 0;
		for (int i = 0; i < var_type.length(); ++i)
			if (var_type[i] == '[' || var_type[i] == '*')
				++dim;
	};
	Variate()
	{
	}
};

#pragma endregion Variate

#pragma region Function

class Function
{
public:
	std::string ret_type;
	std::vector<FuncFParamAST *> params_type;
	Function()
	{
	}
	Function(std::string __ret_type__, std::vector<FuncFParamAST *> __params_type__)
	{
		ret_type = __ret_type__;
		params_type = __params_type__;
	}
};

#pragma endregion Function

#pragma region Symbol

struct Symbol
{
	union
	{
		Variate *var;
		Function *func;
	} data;
	int tag;
	Symbol(Variate __variate__)
	{
		tag = 0;
		data.var = new Variate();
		*data.var = __variate__;
	}
	Symbol(Function __function__)
	{
		tag = 1;
		data.func = new Function();
		*data.func = __function__;
	}
};

#pragma endregion Symbol

#pragma region SymbolTable

class SymbolTable
{
public:
	SymbolTable *father_symbol_table;
	std::map<std::string, std::unique_ptr<Symbol>> map_symbol_table;
	std::vector<std::unique_ptr<SymbolTable>> vec_child_symbol_table;
	SymbolTable *AddSymbolTable()
	{
		std::unique_ptr<SymbolTable> new_symbol_table(new SymbolTable);
		new_symbol_table->father_symbol_table = this;
		vec_child_symbol_table.push_back(std::move(new_symbol_table));
		return vec_child_symbol_table.back().get();
	}
	auto find(std::string ident)
	{
		auto iter = map_symbol_table.find(ident);
		if (iter == map_symbol_table.end())
		{
			if (father_symbol_table == NULL)
				return map_symbol_table.end();
			else
				return father_symbol_table->find(ident);
		}
		else
		{
			return iter;
		}
	}
	auto insert(std::string __ident__, Variate __variate__)
	{
		return map_symbol_table.insert(make_pair(__ident__, std::unique_ptr<Symbol>(new Symbol(__variate__))));
	}
	auto insert(std::string __ident__, Function __function__)
	{
		return map_symbol_table.insert(make_pair(__ident__, std::unique_ptr<Symbol>(new Symbol(__function__))));
	}
	void print_symbol_table()
	{
		auto iter = map_symbol_table.begin();
		if (iter == map_symbol_table.end())
		{
			printf("empty\n");
			return;
		}
		while (iter != map_symbol_table.end())
		{
			printf("%s: ", iter->first.c_str());
			if (iter->second->tag == 0)
				printf("variate\n");
			else
				printf("function\n");
			++iter;
		}
	}
};

#pragma endregion SymbolTable

#pragma region SymbolTableTree

class SymbolTableTree
{
public:
	std::unique_ptr<SymbolTable> symbol_table_tree_root;
};

#pragma endregion SymbolTableTree

#pragma endregion SymbolTable

#pragma region SpaceNeeded

struct SpaceNeeded
{
	int total_needed;
	int return_address_needed;
	int params_needed;
	int variate_needed;
	SpaceNeeded() : total_needed(0), return_address_needed(0), params_needed(0), variate_needed(0){};
};

#pragma endregion SpaceNeeded

#pragma region AST_Op

#pragma region LVal

class LValAST : public BaseAST
{
public:
	union
	{
		LValCase0 *l_val_case_0;
		LValCase1 *l_val_case_1;
	} l_val_union;
	CalData data;
	void Dump(int ctl = 0) override
	{

		if (debug)
			print_dump("LVal", DEBUG_BEGIN);
		++depth_dump;
		switch (tag)
		{
		case (0):
		{
			auto iter = cur_symbol_table->find(l_val_union.l_val_case_0->ident);
			if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
			{
				iter = cur_symbol_table->find(std::string("%") + (l_val_union.l_val_case_0->ident.c_str() + 1));
				if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
				{
					printf("%s: not found\n", l_val_union.l_val_case_0->ident.c_str());
					printf("%s\n", k_str.c_str());
					assert(false);
				}
			}
			if (iter->second->data.var->flag_is_const == IS_CONST)
			{
				data.is_num = true;
				data.count = -1;
				data.val = iter->second->data.var->val;
			}
			else if (iter->second->data.var->var_type == "*i32" || iter->second->data.var->var_type.substr(0, 2) == "**")
			{
				k_str += ("    %" + itostr(unused_koopa_count) + " = load " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + '\n');
				data.is_num = false;
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			}
			else if (iter->second->data.var->var_type.substr(0, 2) == "*[")
			{
				k_str += ("    %" + itostr(unused_koopa_count) + " = getelemptr " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + ", 0\n");
				data.is_num = false;
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			}
			else
			{
				printf("%s\n", iter->second->data.var->var_type.c_str());
				assert(false);
			}
			break;
		}
		case (1):
		{
			auto iter = cur_symbol_table->find(l_val_union.l_val_case_1->ident);
			for (int i = 0; i < l_val_union.l_val_case_1->vec_exp.size(); ++i)
				l_val_union.l_val_case_1->vec_exp[i]->Dump();
			if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
			{
				iter = cur_symbol_table->find(std::string("%") + (l_val_union.l_val_case_0->ident.c_str() + 1));
				if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 0)
					assert(false);
			}
			if (iter->second->data.var->var_type.substr(0, 2) == "**")
			{
				k_str += ("    %" + itostr(unused_koopa_count) + " = load " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + '\n');
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			}
			if (iter->second->data.var->var_type.substr(0, 2) == "*[")
				k_str += "    %" + itostr(unused_koopa_count) + " = getelemptr " + iter->first + '_' + itostr(iter->second->data.var->koopa_var_count) + ", ";
			else if (iter->second->data.var->var_type.substr(0, 2) == "**")
				k_str += "    %" + itostr(unused_koopa_count) + " = getptr %" + itostr(unused_koopa_count - 1) + ", ";
			else
			{
				printf("%s\n", iter->second->data.var->var_type.c_str());
				assert(false);
			}
			l_val_union.l_val_case_1->vec_exp[0]->data.print_data_to_k_str();
			k_str += '\n';
			++unused_koopa_count;
			cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");

			for (int i = 1; i < l_val_union.l_val_case_1->vec_exp.size(); ++i)
			{
				k_str += "    %" + itostr(unused_koopa_count) + " = getelemptr %" + itostr(unused_koopa_count - 1) + ", ";
				l_val_union.l_val_case_1->vec_exp[i]->data.print_data_to_k_str();
				k_str += '\n';
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			}
			if (iter->second->data.var->dim == l_val_union.l_val_case_1->vec_exp.size() + 1)
				k_str += ("    %" + itostr(unused_koopa_count) + " = load " + "%" + itostr(unused_koopa_count - 1) + '\n');
			else
				k_str += ("    %" + itostr(unused_koopa_count) + " = getelemptr %" + itostr(unused_koopa_count - 1) + ", 0\n");
			data.is_num = false;
			data.count = unused_koopa_count;
			++unused_koopa_count;
			cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			break;
		}
		default:
		{
			assert(false);
			break;
		}
		}
		--depth_dump;
		if (debug)
			print_dump("LVal", DEBUG_END);
	}
	LValAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			l_val_union.l_val_case_0 = new LValCase0();
			break;
		case 1:
			l_val_union.l_val_case_1 = new LValCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion LVal

#pragma region LAnd

class LAndOpAST : public BaseOpAST
{
public:
	char op[3];
	void Dump(int ctl = 0) override
	{
		int temp_val = 0;
		if (debug)
			print_dump("LAndOp", DEBUG_BEGIN);
		temp_val = lhs.val && rhs.val;
		if (is_calculating_const_exp == false)
		{

			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ne ";
			if (lhs.is_num == true)
			{
				k_str += itostr(lhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(lhs.count);
			}
			k_str += ", 0\n";
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ne ";
			if (rhs.is_num == true)
			{
				k_str += itostr(rhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(rhs.count);
			}
			k_str += ", 0\n";
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = and %";
			k_str += itostr(unused_koopa_count - 1);
			k_str += ", %";
			k_str += itostr(unused_koopa_count - 2);
			k_str += '\n';
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}
		result.val = temp_val;
		if (debug)
			print_dump("LAndOp", DEBUG_END);
	}
};

#pragma endregion LAnd

#pragma region Unary

class UnaryOpAST : public BaseOpAST
{
public:
	char op;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("UnaryOp", DEBUG_BEGIN);
		printf("%c\n", op);
		if (op == '+')
		{
			result = rhs;
			goto END_OF_DUMP;
		}
		if (is_calculating_const_exp == false)
		{
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ";
		}
		switch (op)
		{
		case ('-'):
		{
			if (is_calculating_const_exp == false)
				k_str += "sub";
			result.val = (-1) * rhs.val;
			break;
		}
		case ('!'):
		{
			if (is_calculating_const_exp == false)
				k_str += "eq";
			result.val = !rhs.val;
			break;
		}
		default:
		{
			assert(false);
		}
		}
		if (is_calculating_const_exp == false)
		{
			if (rhs.is_num == true)
			{

				k_str += " 0, ";
				k_str += itostr(rhs.val);
				k_str += '\n';
			}
			else
			{
				k_str += " 0, %";
				k_str += itostr(rhs.count);
				k_str += '\n';
			}
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			std::cout << k_str << '\n';
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}
	END_OF_DUMP:
		if (debug)
			print_dump("UnaryOp", DEBUG_END);
	}
};

#pragma endregion Unary

#pragma region Mul

class MulOpAST : public BaseOpAST
{
public:
	char op;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("MulOp", DEBUG_BEGIN);
		if (is_calculating_const_exp == false)
		{
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ";
		}
		switch (op)
		{
		case ('*'):
			if (is_calculating_const_exp == false)
				k_str += "mul ";
			else
				result.val = lhs.val * rhs.val;
			break;
		case ('/'):
			if (is_calculating_const_exp == false)
				k_str += "div ";
			else
				result.val = lhs.val / rhs.val;
			break;
		case ('%'):
			if (is_calculating_const_exp == false)
				k_str += "mod ";
			else
				result.val = lhs.val % rhs.val;
			break;
		default:
			assert(false);
		}
		if (is_calculating_const_exp == false)
		{
			if (lhs.is_num == true)
			{
				k_str += itostr(lhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(lhs.count);
			}
			k_str += ", ";
			if (rhs.is_num == true)
			{
				k_str += itostr(rhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(rhs.count);
			}
			k_str += "\n";
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}
		if (debug)
			print_dump("MulOp", DEBUG_END);
	}
};

#pragma endregion Mul

#pragma region Add
class AddOpAST : public BaseOpAST
{
public:
	char op;
	void Dump(int ctl = 0) override
	{
		int temp_val = 0;
		if (debug)
			print_dump("AddOp", DEBUG_BEGIN);
		if (is_calculating_const_exp == false)
		{
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ";
		}
		switch (op)
		{
		case ('+'):
		{
			if (is_calculating_const_exp == false)
				k_str += "add ";
			temp_val = lhs.val + rhs.val;
			break;
		}
		case ('-'):
			if (is_calculating_const_exp == false)
				k_str += "sub ";
			temp_val = lhs.val - rhs.val;
			break;
		default:
			assert(false);
		}
		if (is_calculating_const_exp == false)
		{
			if (lhs.is_num == true)
			{
				k_str += itostr(lhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(lhs.count);
			}
			k_str += ", ";
			if (rhs.is_num == true)
			{
				k_str += itostr(rhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(rhs.count);
			}
			k_str += "\n";
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}
		result.val = temp_val;
		if (debug)
			print_dump("AddOp", DEBUG_END);
	}
};

#pragma endregion Add

#pragma region Rel

class RelOpAST : public BaseOpAST
{
public:
	// tag 0:>|<;tag 1??>=|<=
	char op[3];
	void Dump(int ctl = 0) override
	{
		int temp_val = 0;
		if (debug)
			print_dump("RelOp", DEBUG_BEGIN);
		if (is_calculating_const_exp == false)
		{
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ";
		}
		if (str_is_same(op, ">") == true)
		{
			if (is_calculating_const_exp == false)
				k_str += "gt ";
			temp_val = lhs.val > rhs.val;
		}
		else if (str_is_same(op, "<") == true)
		{
			if (is_calculating_const_exp == false)
				k_str += "lt ";
			temp_val = lhs.val < rhs.val;
		}
		else if (str_is_same(op, ">=") == true)
		{
			if (is_calculating_const_exp == false)
				k_str += "ge ";
			temp_val = lhs.val >= rhs.val;
		}
		else if (str_is_same(op, "<=") == true)
		{
			if (is_calculating_const_exp == false)
				k_str += "le ";
			temp_val = lhs.val <= rhs.val;
		}
		if (is_calculating_const_exp == false)
		{
			if (lhs.is_num == true)
			{
				k_str += itostr(lhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(lhs.count);
			}
			k_str += ", ";
			if (rhs.is_num == true)
			{
				k_str += itostr(rhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(rhs.count);
			}
			k_str += "\n";
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}
		result.val = temp_val;
		if (debug)
			print_dump("RelOp", DEBUG_END);
	}
};

#pragma endregion Rel

#pragma region LOr

class LOrOpAST : public BaseOpAST
{
public:
	char op[3];
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("LOrOp", DEBUG_BEGIN);
		result.val = lhs.val || rhs.val;
		if (is_calculating_const_exp == false)
		{
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = or ";
			if (lhs.is_num == true)
			{
				k_str += itostr(lhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(lhs.count);
			}
			k_str += ", ";
			if (rhs.is_num == true)
			{
				k_str += itostr(rhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(rhs.count);
			}
			k_str += "\n";
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ne %";
			k_str += itostr(unused_koopa_count - 1);
			k_str += ", ";
			k_str += itostr(0);
			k_str += '\n';
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}

		if (debug)
			print_dump("LOrOp", DEBUG_END);
	}
};

#pragma endregion LOr

#pragma region Eq

class EqOpAST : public BaseOpAST
{
public:
	char op[3];
	void Dump(int ctl = 0) override
	{
		int temp_val = 0;
		if (debug)
			print_dump("EqOp", DEBUG_BEGIN);
		if (is_calculating_const_exp == false)
		{
			k_str += "    %";
			k_str += itostr(unused_koopa_count);
			k_str += " = ";
		}
		if (str_is_same(op, "==") == true)
		{
			if (is_calculating_const_exp == false)
				k_str += "eq ";
			temp_val = lhs.val == rhs.val;
		}
		else
		{
			if (is_calculating_const_exp == false)
				k_str += "ne ";
			temp_val = lhs.val != rhs.val;
		}
		if (is_calculating_const_exp == false)
		{
			if (lhs.is_num == true)
			{
				k_str += itostr(lhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(lhs.count);
			}
			k_str += ", ";
			if (rhs.is_num == true)
			{
				k_str += itostr(rhs.val);
			}
			else
			{
				k_str += '%';
				k_str += itostr(rhs.count);
			}
			k_str += "\n";
			result.is_num = false;
			result.count = unused_koopa_count;
			++unused_koopa_count;
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
		}
		result.val = temp_val;
		if (debug)
			print_dump("EqOp", DEBUG_END);
	}
};

#pragma endregion Eq

#pragma endregion AST_Op

#pragma region AST_Exp

#pragma region Number

class NumberAST : public BaseExpAST
{
public:
	std::string num;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("Number", DEBUG_BEGIN);
		data.is_num = true;
		data.val = strtoi(num);
		data.count = -1;
		if (debug)
			print_dump("Number", DEBUG_END);
	}
};

#pragma endregion Number

#pragma region Primary

class PrimaryExpAST : public BaseExpAST
{
public:
	union
	{
		PrimaryExpCase0 *primary_exp_case_0;
		PrimaryExpCase1 *primary_exp_case_1;
		PrimaryExpCase2 *primary_exp_case_2;
	} primary_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("PrimaryExp", DEBUG_BEGIN);
		switch (tag)
		{
		case 0: // primaryexp
		{
			++depth_dump;
			primary_exp_union.primary_exp_case_0->exp->Dump();
			data = primary_exp_union.primary_exp_case_0->exp->data;
			--depth_dump;
			break;
		}
		case 1:
		{
			++depth_dump;
			primary_exp_union.primary_exp_case_1->l_val->Dump();
			--depth_dump;
			data = dynamic_cast<LValAST *>(primary_exp_union.primary_exp_case_1->l_val.get())->data;
			break;
		}
		case 2: // unary
		{
			++depth_dump;
			primary_exp_union.primary_exp_case_2->number->Dump();
			data = primary_exp_union.primary_exp_case_2->number->data;
			--depth_dump;
			break;
		}
		default:
		{
			assert(false);
		}
		}
		if (debug)
			print_dump("PrimaryExp", DEBUG_END);
	}
	PrimaryExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			primary_exp_union.primary_exp_case_0 = new PrimaryExpCase0();
			break;
		case 1:
			primary_exp_union.primary_exp_case_1 = new PrimaryExpCase1();
			break;
		case 2:
			primary_exp_union.primary_exp_case_2 = new PrimaryExpCase2();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Primary

#pragma region Unary

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
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("UnaryExp", DEBUG_BEGIN);
		switch (tag)
		{
		case 0: // primaryexp
		{
			++depth_dump;
			unary_exp_union.unary_exp_case_0->primary_exp->Dump();
			data = unary_exp_union.unary_exp_case_0->primary_exp->data;
			--depth_dump;
			break;
		}
		case 1:
		{
			auto iter = cur_symbol_table->find(unary_exp_union.unary_exp_case_1->ident);
			if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end())
			{
				symbol_table_tree.symbol_table_tree_root->print_symbol_table();
				cur_symbol_table->father_symbol_table->print_symbol_table();
				assert(false);
			}
			if (iter == symbol_table_tree.symbol_table_tree_root->map_symbol_table.end() || iter->second->tag != 1)
				assert(false);
			cur_map_iter_for_func_space_needed->second.return_address_needed = 4;
			if (iter->second->data.func->ret_type == "void")
			{
				unary_exp_union.unary_exp_case_1->func_r_params->Dump(1);
				k_str += ("    call " + iter->first + "(");
				unary_exp_union.unary_exp_case_1->func_r_params->Dump();
				k_str += ")\n";
				data.count = -1;
				data.is_num = false;
				data.val = -1;
			}
			else
			{
				unary_exp_union.unary_exp_case_1->func_r_params->Dump(1);
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				data.is_num = false;
				data.val = -1;
				k_str += ("    %" + itostr(data.count) + " = call " + iter->first + '(');
				unary_exp_union.unary_exp_case_1->func_r_params->Dump();
				k_str += ")\n";
			}
			break;
		}
		case 2: // unary
		{
			++depth_dump;
			unary_exp_union.unary_exp_case_2->unary_exp->Dump();
			unary_exp_union.unary_exp_case_2->unary_op->rhs = unary_exp_union.unary_exp_case_2->unary_exp->data;
			--depth_dump;
			++depth_dump;
			unary_exp_union.unary_exp_case_2->unary_op->Dump();
			data = unary_exp_union.unary_exp_case_2->unary_op->result;
			--depth_dump;
			break;
		}
		default:
			assert(false);
		}
		if (debug)
			print_dump("UnaryExp", DEBUG_END);
	}
	UnaryExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			unary_exp_union.unary_exp_case_0 = new UnaryExpCase0();
			break;
		case 1:
			unary_exp_union.unary_exp_case_1 = new UnaryExpCase1();
			break;
		case 2:
			unary_exp_union.unary_exp_case_2 = new UnaryExpCase2();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Unary

#pragma region Mul

class MulExpAST : public BaseExpAST
{
public:
	union
	{
		MulExpCase0 *mul_exp_case_0;
		MulExpCase1 *mul_exp_case_1;
	} mul_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("MulExp", DEBUG_BEGIN);
		switch (tag)
		{
		case 0:
			// unaryexp
			++depth_dump;
			mul_exp_union.mul_exp_case_0->unary_exp->Dump();
			data = mul_exp_union.mul_exp_case_0->unary_exp->data;
			--depth_dump;
			break;
		case 1:
			++depth_dump;
			mul_exp_union.mul_exp_case_1->unary_exp->Dump();
			mul_exp_union.mul_exp_case_1->mul_op.get()->rhs = mul_exp_union.mul_exp_case_1->unary_exp->data;
			--depth_dump;
			++depth_dump;
			mul_exp_union.mul_exp_case_1->mul_exp->Dump();
			mul_exp_union.mul_exp_case_1->mul_op.get()->lhs = mul_exp_union.mul_exp_case_1->mul_exp->data;
			--depth_dump;
			++depth_dump;
			mul_exp_union.mul_exp_case_1->mul_op->Dump();
			data = mul_exp_union.mul_exp_case_1->mul_op->result;
			--depth_dump;
			break;
		default:
			assert(false);
		}
		if (debug)
			print_dump("MulExp", DEBUG_END);
	}
	MulExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			mul_exp_union.mul_exp_case_0 = new MulExpCase0();
			break;
		case 1:
			mul_exp_union.mul_exp_case_1 = new MulExpCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Mul

#pragma region Add

class AddExpAST : public BaseExpAST
{
public:
	union
	{
		AddExpCase0 *add_exp_case_0;
		AddExpCase1 *add_exp_case_1;
	} add_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("AddExp", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			// unaryexp
			++depth_dump;
			add_exp_union.add_exp_case_0->mul_exp->Dump();
			data = add_exp_union.add_exp_case_0->mul_exp->data;
			--depth_dump;
			break;
		}
		case (1):
		{
			++depth_dump;
			add_exp_union.add_exp_case_1->mul_exp->Dump();
			add_exp_union.add_exp_case_1->add_op->rhs = add_exp_union.add_exp_case_1->mul_exp->data;
			--depth_dump;
			++depth_dump;
			add_exp_union.add_exp_case_1->add_exp->Dump();
			add_exp_union.add_exp_case_1->add_op->lhs = add_exp_union.add_exp_case_1->add_exp->data;
			--depth_dump;
			++depth_dump;
			add_exp_union.add_exp_case_1->add_op->Dump();
			data = add_exp_union.add_exp_case_1->add_op->result;
			--depth_dump;
			break;
		}
		}
		if (debug)
			print_dump("AddExp", DEBUG_END);
	}
	AddExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			add_exp_union.add_exp_case_0 = new AddExpCase0();
			break;
		case 1:
			add_exp_union.add_exp_case_1 = new AddExpCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Add

#pragma region Rel

class RelExpAST : public BaseExpAST
{
public:
	union
	{
		RelExpCase0 *rel_exp_case_0;
		RelExpCase1 *rel_exp_case_1;
	} rel_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("RelExp", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			++depth_dump;
			rel_exp_union.rel_exp_case_0->add_exp->Dump();
			data = rel_exp_union.rel_exp_case_0->add_exp->data;
			--depth_dump;
			break;
		}
		case (1):
		{
			++depth_dump;
			rel_exp_union.rel_exp_case_1->add_exp->Dump();
			rel_exp_union.rel_exp_case_1->rel_op->rhs = rel_exp_union.rel_exp_case_1->add_exp->data;
			--depth_dump;
			++depth_dump;
			rel_exp_union.rel_exp_case_1->rel_exp->Dump();
			rel_exp_union.rel_exp_case_1->rel_op->lhs = rel_exp_union.rel_exp_case_1->rel_exp->data;
			--depth_dump;
			++depth_dump;
			rel_exp_union.rel_exp_case_1->rel_op->Dump();
			data = rel_exp_union.rel_exp_case_1->rel_op->result;
			break;
		}
		default:
		{
			assert(false);
			break;
		}
		}
		if (debug)
			print_dump("RelExp", DEBUG_END);
	}
	RelExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			rel_exp_union.rel_exp_case_0 = new RelExpCase0();
			break;
		case 1:
			rel_exp_union.rel_exp_case_1 = new RelExpCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Rel

#pragma region Eq

class EqExpAST : public BaseExpAST
{
public:
	union
	{
		EqExpCase0 *eq_exp_case_0;
		EqExpCase1 *eq_exp_case_1;
	} eq_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("EqExp", DEBUG_BEGIN);
		switch (tag)
		{
		case 0:
			++depth_dump;
			eq_exp_union.eq_exp_case_0->rel_exp->Dump();
			data = eq_exp_union.eq_exp_case_0->rel_exp->data;
			--depth_dump;
			break;
		case 1:
			++depth_dump;
			eq_exp_union.eq_exp_case_1->rel_exp->Dump();
			eq_exp_union.eq_exp_case_1->eq_op->rhs = eq_exp_union.eq_exp_case_1->rel_exp->data;
			--depth_dump;
			++depth_dump;
			eq_exp_union.eq_exp_case_1->eq_exp->Dump();
			eq_exp_union.eq_exp_case_1->eq_op->lhs = eq_exp_union.eq_exp_case_1->eq_exp->data;
			--depth_dump;
			++depth_dump;
			eq_exp_union.eq_exp_case_1->eq_op->Dump();
			data = eq_exp_union.eq_exp_case_1->eq_op->result;
			break;
		default:
			assert(false);
		}
		if (debug)
			print_dump("EqExp", DEBUG_END);
	}
	EqExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			eq_exp_union.eq_exp_case_0 = new EqExpCase0();
			break;
		case 1:
			eq_exp_union.eq_exp_case_1 = new EqExpCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Eq

#pragma region LAnd

class LAndExpAST : public BaseExpAST
{
public:
	union
	{
		LAndExpCase0 *l_and_exp_case_0;
		LAndExpCase1 *l_and_exp_case_1;
	} l_and_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("LAndExp", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			++depth_dump;
			l_and_exp_union.l_and_exp_case_0->eq_exp->Dump();
			data = l_and_exp_union.l_and_exp_case_0->eq_exp->data;
			--depth_dump;
			break;
		}
		case (1):
		{
			//%x = alloc T
			std::string temp_logic_result = "%temp_l_result_" + itostr(unused_koopa_var_count); //???????��?????��????
			if (is_calculating_const_exp == false)
			{
				++unused_koopa_var_count;
				k_str += (std::string("  ") + temp_logic_result + " = alloc i32\n");
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			}
			++depth_dump;
			l_and_exp_union.l_and_exp_case_1->l_and_exp->Dump();
			l_and_exp_union.l_and_exp_case_1->l_and_op->lhs = l_and_exp_union.l_and_exp_case_1->l_and_exp->data;
			;
			--depth_dump;
			std::string label_then = "%then_" + itostr(unused_koopa_label_count); //???????��?????��????
			if (is_calculating_const_exp == false)
				++unused_koopa_label_count;
			std::string label_else = "%else_" + itostr(unused_koopa_label_count); //???????��?????��????
			if (is_calculating_const_exp == false)
				++unused_koopa_label_count;
			std::string label_end = "%end_" + itostr(unused_koopa_label_count); //???????��?????��????
			if (is_calculating_const_exp == false)
				++unused_koopa_label_count;
			if (is_calculating_const_exp == false)
			{
				if (l_and_exp_union.l_and_exp_case_1->l_and_op->lhs.is_num == false)
				{
					k_str += "    br %";
					k_str += itostr(l_and_exp_union.l_and_exp_case_1->l_and_op->lhs.count);
				}
				else
				{
					k_str += "    br ";
					k_str += itostr(l_and_exp_union.l_and_exp_case_1->l_and_op->lhs.val);
				}
				k_str += (", " + label_then + ", " + label_else + '\n');
				k_str += ("  " + label_then + ":\n");
			}
			// lhs?????????rhs,??label_then
			++depth_dump;
			l_and_exp_union.l_and_exp_case_1->eq_exp->Dump();
			l_and_exp_union.l_and_exp_case_1->l_and_op->rhs = l_and_exp_union.l_and_exp_case_1->eq_exp->data;
			--depth_dump;
			++depth_dump;
			l_and_exp_union.l_and_exp_case_1->l_and_op->Dump();
			data = l_and_exp_union.l_and_exp_case_1->l_and_op->result;
			--depth_dump;
			// store %x, %y
			if (is_calculating_const_exp == false)
			{
				k_str += ("  store %" + itostr(data.count) + ", " + temp_logic_result + '\n');
				if (last_ins_is_ret == false)
					k_str += ("    jump " + label_end + '\n');
				else
					last_ins_is_ret = false;
				k_str += ("  " + label_else + ":\n");
				k_str += (std::string("  store 0") + ", " + temp_logic_result + '\n');
				data.is_num = false;
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				if (last_ins_is_ret == false)
					k_str += ("    jump " + label_end + '\n');
				else
					last_ins_is_ret = false;
				k_str += ("  " + label_end + ':');
				k_str += '\n';
				//% x = load % y
				k_str += ("    %" + itostr(unused_koopa_count) + " = load " + temp_logic_result + '\n');
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				data.is_num = false;
			}
			break;
		}
		default:
			assert(false);
		}
		if (debug)
			print_dump("LAndExp", DEBUG_END);
	}
	LAndExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			l_and_exp_union.l_and_exp_case_0 = new LAndExpCase0();
			break;
		case 1:
			l_and_exp_union.l_and_exp_case_1 = new LAndExpCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion LAnd

#pragma region LOr

class LOrExpAST : public BaseExpAST
{
public:
	union
	{
		LOrExpCase0 *l_or_exp_case_0;
		LOrExpCase1 *l_or_exp_case_1;
	} l_or_exp_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("LOrExp", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			++depth_dump;
			l_or_exp_union.l_or_exp_case_0->l_and_exp->Dump();
			data = l_or_exp_union.l_or_exp_case_0->l_and_exp->data;
			--depth_dump;
			break;
		}
		case (1):
		{
			std::string temp_logic_result = "%temp_l_result_" + itostr(unused_koopa_var_count); //???????��?????��????
			if (is_calculating_const_exp == false)
			{

				++unused_koopa_var_count;
				k_str += (std::string("  ") + temp_logic_result + " = alloc i32\n");
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
			}
			++depth_dump;
			l_or_exp_union.l_or_exp_case_1->l_or_exp->Dump();
			l_or_exp_union.l_or_exp_case_1->l_or_op->lhs = l_or_exp_union.l_or_exp_case_1->l_or_exp->data;
			--depth_dump;
			std::string label_then = "%then_" + itostr(unused_koopa_label_count); //???????��?????��????
			if (is_calculating_const_exp == false)
				++unused_koopa_label_count;
			std::string label_else = "%else_" + itostr(unused_koopa_label_count); //???????��?????��????
			if (is_calculating_const_exp == false)
				++unused_koopa_label_count;
			std::string label_end = "%end_" + itostr(unused_koopa_label_count); //???????��?????��????
			if (is_calculating_const_exp == false)
				++unused_koopa_label_count;
			if (is_calculating_const_exp == false)
			{
				if (l_or_exp_union.l_or_exp_case_1->l_or_exp->data.is_num == false)
				{
					k_str += "    br %";
					k_str += itostr(l_or_exp_union.l_or_exp_case_1->l_or_exp->data.count);
				}
				else
				{
					k_str += "    br ";
					k_str += itostr(l_or_exp_union.l_or_exp_case_1->l_or_exp->data.val);
				}
				k_str += (", " + label_then + ", " + label_else + '\n');
				k_str += ("  " + label_else + ":\n");
			}
			// lhs?????????rhs,??label_then
			++depth_dump;
			l_or_exp_union.l_or_exp_case_1->l_and_exp->Dump();
			l_or_exp_union.l_or_exp_case_1->l_or_op->rhs = l_or_exp_union.l_or_exp_case_1->l_and_exp->data;
			--depth_dump;
			++depth_dump;
			l_or_exp_union.l_or_exp_case_1->l_or_op->Dump();
			--depth_dump;
			data = l_or_exp_union.l_or_exp_case_1->l_or_op->result;
			// store %x, %y
			if (is_calculating_const_exp == false)
			{
				k_str += ("  store %" + itostr(data.count) + ", " + temp_logic_result + '\n');
				if (last_ins_is_ret == false)
					k_str += ("    jump " + label_end + '\n');
				else
					last_ins_is_ret = false;
				k_str += ("  " + label_then + ":\n");
				k_str += (std::string("  store 1") + ", " + temp_logic_result + '\n');
				data.is_num = false;
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				if (last_ins_is_ret == false)
					k_str += ("    jump " + label_end + '\n');
				else
					last_ins_is_ret = false;
				k_str += ("  " + label_end + ':');
				k_str += '\n';
				//% x = load % y
				k_str += ("    %" + itostr(unused_koopa_count) + " = load " + temp_logic_result + '\n');
				data.count = unused_koopa_count;
				++unused_koopa_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
				data.is_num = false;
			}
			break;
		}
		default:
			assert(false);
		}
		if (debug)
			print_dump("LOrExp", DEBUG_END);
	}
	LOrExpAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			l_or_exp_union.l_or_exp_case_0 = new LOrExpCase0();
			break;
		case 1:
			l_or_exp_union.l_or_exp_case_1 = new LOrExpCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion LOr

#pragma region Simple

class ExpAST : public BaseExpAST
{
public:
	std::unique_ptr<BaseExpAST> l_or_exp;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("Exp", DEBUG_BEGIN);
		++depth_dump;
		if (cur_symbol_table == symbol_table_tree.symbol_table_tree_root.get())
			is_calculating_const_exp = true;
		l_or_exp->Dump();
		is_calculating_const_exp = false;
		data = l_or_exp->data;
		--depth_dump;
		if (debug)
			print_dump("Exp", DEBUG_END);
	}
};

#pragma endregion Simple

#pragma region Const

class ConstExpAST : public BaseExpAST
{
public:
	std::unique_ptr<BaseExpAST> exp;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("ConstExp", DEBUG_BEGIN);
		++depth_dump;
		exp->Dump();
		data = exp->data;
		--depth_dump;
		if (debug)
			print_dump("ConstExp", DEBUG_END);
	}
};

#pragma endregion Const

#pragma endregion AST_Exp

#pragma region AST_Other

#pragma region BType

class BTypeAST : public BaseTypeAST
{
public:
	BTypeAST(std::string __type__) { type = __type__; };
	BTypeAST(){};
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("BType", DEBUG_BEGIN);
		if (type == "int")
		{
			// k_str += "i32";
		}
		if (debug)
			print_dump("BType", DEBUG_END);
	}
};

#pragma endregion BType

#pragma region ConstInitVal

class ConstInitValAST : public BaseAST
{
public:
	union
	{
		ConstInitValCase0 *const_init_val_case_0;
		ConstInitValCase1 *const_init_val_case_1;
	} const_init_val_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("ConstInitVal", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			++depth_dump;
			is_calculating_const_exp = true;
			const_init_val_union.const_init_val_case_0->const_exp->Dump();
			is_calculating_const_exp = false;
			const_init_val_union.const_init_val_case_0->data = const_init_val_union.const_init_val_case_0->const_exp->data;
			--depth_dump;
			break;
		}
		case (1):
		{
			for (int i = 0; i < const_init_val_union.const_init_val_case_1->vec_const_init_val.size(); ++i)
			{
				++depth_dump;
				const_init_val_union.const_init_val_case_1->vec_const_init_val[i]->Dump();
				// const_init_val_union.const_init_val_case_1->vec_data.push_back(const_init_val_union.const_init_val_case_1->vec_const_exp[i]->data);
				--depth_dump;
			}
			break;
		}
		default:
			assert(false);
			break;
		}
		if (debug)
			print_dump("ConstInitVal", DEBUG_END);
	}
	ConstInitValAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			const_init_val_union.const_init_val_case_0 = new ConstInitValCase0();
			const_init_val_union.const_init_val_case_0->data.val = 0;
			const_init_val_union.const_init_val_case_0->data.is_num = true;
			break;
		case 1:
			const_init_val_union.const_init_val_case_1 = new ConstInitValCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion ConstInitVal

void ConstInitValInit(std::vector<std::unique_ptr<BaseExpAST>>::iterator iter_begin, std::vector<std::unique_ptr<BaseExpAST>>::iterator iter_end, InitList *civ);

#pragma region ConstDef

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
	ConstDefAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			const_def_union.const_def_case_0 = new ConstDefCase0();
			break;
		case 1:
			const_def_union.const_def_case_1 = new ConstDefCase1();
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
			if (dim == const_def_union.const_def_case_1->vec_const_exp.size())
			{
				if (cur_init_ele_koopa_list->tag == 1)
					assert(false);
				cur_init_ele_koopa_list->init_list_union.init_list_case_0->data.print_data_to_k_str();
				return;
			}
			int vec_const_exp_dim_len = const_def_union.const_def_case_1->vec_const_exp[dim]->data.val;
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
			if (dim == const_def_union.const_def_case_1->vec_const_exp.size())
			{
				if (cur_init_ele_koopa_list->tag == 1)
					assert(false);
				k_str += "    store ";
				cur_init_ele_koopa_list->init_list_union.init_list_case_0->data.print_data_to_k_str();
				k_str += (", " + str_src + '\n');
				return;
			}
			int vec_const_exp_dim_len = const_def_union.const_def_case_1->vec_const_exp[dim]->data.val;
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
		if (debug)
			print_dump("ConstDef", DEBUG_BEGIN);
		switch (tag)
		{
		case (0):
		{
			++depth_dump;
			size = 0; //????????????????
			const_def_union.const_def_case_0->const_init_val->Dump();
			const_def_union.const_def_case_0->data = dynamic_cast<ConstInitValAST *>(const_def_union.const_def_case_0->const_init_val.get())->const_init_val_union.const_init_val_case_0->data;
			--depth_dump;
			cur_symbol_table->insert(const_def_union.const_def_case_0->ident, Variate(IS_CONST, const_def_union.const_def_case_0->data.val, "int", STRONG));
			++unused_koopa_var_count;
			break;
		}
		case (1):
		{
			size = 4;
			for (int i = 0; i < const_def_union.const_def_case_1->vec_const_exp.size(); ++i)
			{
				is_calculating_const_exp = true;
				const_def_union.const_def_case_1->vec_const_exp[i]->Dump();
				size *= const_def_union.const_def_case_1->vec_const_exp[i]->data.val;
				is_calculating_const_exp = false;
			}
			const_def_union.const_def_case_1->const_init_val->Dump();
			Init();
			Convert(dynamic_cast<ConstInitValAST *>(const_def_union.const_def_case_1->const_init_val.get()));
			cur_init_ele_koopa_list = init_list.get();
			std::string arr_type = const_def_union.const_def_case_1->return_type();
			if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get()) // not global
			{
				auto pair_ident = cur_symbol_table->insert(const_def_union.const_def_case_1->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + arr_type, STRONG));
				std::string str_src = const_def_union.const_def_case_1->ident + '_' + itostr(pair_ident.first->second->data.var->koopa_var_count);
				k_str += ("    " + str_src + " = alloc " + arr_type + "\n");
				++unused_koopa_var_count;
				cur_map_iter_for_func_space_needed->second.variate_needed += size;
				InitLocalArrayInKoopa(DIM_0, str_src);
			}
			else
			{
				cur_symbol_table->insert(const_def_union.const_def_case_1->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + arr_type, GLOBAL));
				k_str += ("global " + const_def_union.const_def_case_1->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + arr_type + ", {");
				global_var_name_queue.push(const_def_union.const_def_case_1->ident);
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
		if (debug)
			print_dump("ConstDef", DEBUG_END);
	}
	void Init()
	{

		if (tag == 0) // not array
		{
		}
		else
		{
			auto iter_begin = const_def_union.const_def_case_1->vec_const_exp.begin();
			auto iter_end = const_def_union.const_def_case_1->vec_const_exp.end();
			init_list = std::unique_ptr<InitList>(new InitList(TAG_1));
			ConstInitValInit(iter_begin, iter_end, init_list.get());
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
	void Convert(ConstInitValAST *civ)
	{
		if (civ->tag == 0)
		{
			auto iter = vec_cur_init_list.end() - 1;
			if ((*(*iter))->tag != 0)
				assert(false);
			(*(*iter))->init_list_union.init_list_case_0->data = civ->const_init_val_union.const_init_val_case_0->data;
			++(vec_cur_init_list.back());
			int index = vec_cur_init_list.size() - 1;
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
			int civ_size = civ->const_init_val_union.const_init_val_case_1->vec_const_init_val.size();
			for (int i = 0; i < civ_size; ++i)
			{
				Convert(dynamic_cast<ConstInitValAST *>(civ->const_init_val_union.const_init_val_case_1->vec_const_init_val[i].get()));
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
			for (int i = index + 1; i < vec_cur_init_list.size(); ++i)
			{
				vec_cur_init_list[i] = (*(vec_cur_init_list[i - 1]))->init_list_union.init_list_case_1->vec_init_list.begin();
				vec_begin_init_list[i] = vec_cur_init_list[i];
				vec_end_init_list[i] = (*(vec_cur_init_list[i - 1]))->init_list_union.init_list_case_1->vec_init_list.end();
			}
		}
	}
};

#pragma endregion ConstDef

#pragma region ConstDecl

class ConstDeclAST : public BaseAST
{
public:
	std::unique_ptr<BaseTypeAST> b_type;
	std::vector<std::unique_ptr<BaseAST>> vec_const_def;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("ConstDecl", DEBUG_BEGIN);
		++depth_dump;
		b_type->Dump();
		--depth_dump;
		++depth_dump;
		if (is_calculating_const_exp == false)
			assert(false);
		for (auto iter = vec_const_def.begin(); iter != vec_const_def.end(); ++iter)
		{
			(*iter)->Dump();
		}
		--depth_dump;
		if (debug)
			print_dump("ConstDecl", DEBUG_END);
	}
};

#pragma endregion ConstDecl

#pragma region Decl

class DeclAST : public BaseAST
{
public:
	union
	{
		DeclCase0 *decl_case_0;
		DeclCase1 *decl_case_1;
	} decl_union;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("Decl", DEBUG_BEGIN);
		++depth_dump;
		switch (tag)
		{
		case (0):
		{
			is_calculating_const_exp = true;
			if (is_calculating_const_exp == false)
				assert(false);
			decl_union.decl_case_0->const_decl->Dump();
			is_calculating_const_exp = false;
			break;
		}
		case (1):
		{
			is_calculating_const_exp = false;
			decl_union.decl_case_1->var_decl->Dump();
			break;
		}
		}
		--depth_dump;
		if (debug)
			print_dump("Decl", DEBUG_END);
	}
	DeclAST(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			decl_union.decl_case_0 = new DeclCase0();
			break;
		case 1:
			decl_union.decl_case_1 = new DeclCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion Decl

#pragma region CompUnitEle

class CompUnitEle : public BaseAST
{
public:
	union
	{
		CompUnitEleCase0 *comp_unit_ele_case_0;
		CompUnitEleCase1 *comp_unit_ele_case_1;
	} comp_unit_ele_union;
	void Dump(int ctl = 0) override
	{
		if (tag == 0)
			comp_unit_ele_union.comp_unit_ele_case_0->func_def->Dump();
		else
			comp_unit_ele_union.comp_unit_ele_case_1->decl->Dump();
	}
	CompUnitEle(int __tag__)
	{
		tag = __tag__;
		switch (tag)
		{
		case 0:
			comp_unit_ele_union.comp_unit_ele_case_0 = new CompUnitEleCase0();
			break;
		case 1:
			comp_unit_ele_union.comp_unit_ele_case_1 = new CompUnitEleCase1();
			break;
		default:
			assert(false);
		}
		return;
	}
};

#pragma endregion CompUnitEle

#pragma region CompUnit

class CompUnitAST : public BaseAST
{
public:
	// ????????????????
	std::unique_ptr<CompUnitEle> comp_unit_ele;
	std::unique_ptr<BaseAST> next_comp_unit_ele;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("CompUnit", DEBUG_BEGIN);
		++depth_dump;
		comp_unit_ele->Dump();
		if (next_comp_unit_ele != NULL)
			next_comp_unit_ele->Dump();
		--depth_dump;
		if (debug)
			print_dump("CompUnit", DEBUG_END);
	}
};

#pragma endregion CompUnit

#pragma region FuncFParam

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
	FuncFParamAST(int __tag__, std::string __ident__ = "")
	{
		tag = __tag__;
		switch (tag)
		{
		case (0):
		{
			func_f_param_union.func_f_param_case_0 = new FuncFParamCase0();
			func_f_param_union.func_f_param_case_0->ident = __ident__;
			break;
		}
		case (1):
		{
			func_f_param_union.func_f_param_case_1 = new FuncFParamCase1();
			func_f_param_union.func_f_param_case_1->ident = __ident__;
			break;
		}
		default:
			assert(false);
		}
	};

	void Dump(int ctl = 0) override
	{
		switch (tag)
		{
		case (0):
		{
			k_str += (std::string("%") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + ": ");
			if (func_f_param_union.func_f_param_case_0->b_type->type == "int")
			{
				koopa_type = "i32";
				k_str += koopa_type;
				size = 4;
			}
			else
				assert(false);
			break;
		}
		case (1):
		{
			size = 4;
			for (int i = 0; i < func_f_param_union.func_f_param_case_1->vec_const_exp.size(); ++i)
			{
				is_calculating_const_exp = true;
				func_f_param_union.func_f_param_case_1->vec_const_exp[i]->Dump();
				is_calculating_const_exp = false;
			}
			koopa_type = func_f_param_union.func_f_param_case_1->return_type();
			k_str += (std::string("%") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + ": " + koopa_type);
			break;
		}
		default:
			assert(false);
		}
	}
	void declare_alloc()
	{
		switch (tag)
		{
		case (0):
		{
			cur_map_iter_for_func_space_needed->second.variate_needed += size;
			cur_symbol_table->insert(func_f_param_union.func_f_param_case_0->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + koopa_type, STRONG));
			k_str += ("    " + func_f_param_union.func_f_param_case_0->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + koopa_type + "\n");
			k_str += (std::string("    store %") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + std::string(", ") + func_f_param_union.func_f_param_case_0->ident.c_str() + '_' + itostr(unused_koopa_var_count) + '\n');
			++unused_koopa_var_count;
			break;
		}
		case (1):
		{
			cur_map_iter_for_func_space_needed->second.variate_needed += size;
			cur_symbol_table->insert(func_f_param_union.func_f_param_case_1->ident, Variate(IS_NOT_CONST, UN_DEF, "*" + koopa_type, STRONG));
			k_str += ("    " + func_f_param_union.func_f_param_case_1->ident + '_' + itostr(unused_koopa_var_count) + " = alloc " + koopa_type + "\n");
			k_str += (std::string("    store %") + (func_f_param_union.func_f_param_case_0->ident.c_str() + 1) + std::string(", ") + func_f_param_union.func_f_param_case_1->ident.c_str() + '_' + itostr(unused_koopa_var_count) + '\n');
			++unused_koopa_var_count;
			break;
		}
		default:
			assert(false);
		}
	}
};

#pragma endregion FuncFParam

#pragma region FuncFParams

class FuncFParamsAST : public BaseAST
{
public:
	std::vector<FuncFParamAST *> func_f_params;
	void Dump(int ctl = 0) override
	{
		if (func_f_params.empty())
			return;
		std::vector<FuncFParamAST *>::iterator iter = func_f_params.begin();
		(*iter)->Dump();
		++iter;
		while (iter != func_f_params.end())
		{
			k_str += ',';
			(*iter)->Dump();
			++iter;
		}
	}
	void declare_alloc()
	{
		auto iter = func_f_params.begin();
		while (iter != func_f_params.end())
		{
			(*iter)->declare_alloc();
			++iter;
		}
	}
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
		if (debug)
			print_dump("FuncType", DEBUG_BEGIN);
		if (type == "int")
			k_str += ": i32\n";
		else if (type == "void")
			k_str += '\n';
		if (debug)
			print_dump("FuncType", DEBUG_END);
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
		if (debug)
			print_dump("FuncDef", DEBUG_BEGIN);
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
		if (debug)
			print_dump("FuncDef", DEBUG_END);
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
		if (debug)
			print_dump("Block", DEBUG_BEGIN);
		for (auto iter = vec_block_item.begin(); iter != vec_block_item.end(); ++iter)
		{
			++depth_dump;
			(*iter)->Dump();
			--depth_dump;
		}
		if (debug)
			print_dump("Block", DEBUG_END);
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
		if (debug)
		{
			print_dump("Stmt", DEBUG_BEGIN);
			print_ident(depth_dump);
			printf("tag:%d\n", tag);
		}
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
		if (debug)
			print_dump("Stmt", DEBUG_END);
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
		if (debug)
			print_dump("InitVal", DEBUG_BEGIN);
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
		if (debug)
			print_dump("InitVal", DEBUG_END);
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

#pragma endregion InitVal

#pragma region BlockItem

class BlockItemAST : public BaseAST
{
public:
	// tag 0
	std::unique_ptr<BaseAST> decl;
	// tag 1
	std::unique_ptr<BaseAST> stmt;
	void Dump(int ctl = 0) override
	{
		if (debug)
			print_dump("BlockItem", DEBUG_BEGIN);
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
		if (debug)
			print_dump("BlockItem", DEBUG_END);
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
		if (debug)
			print_dump("VarDecl", DEBUG_BEGIN);
		++depth_dump;
		b_type->Dump();
		--depth_dump;
		++depth_dump;
		for (auto iter = vec_var_def.begin(); iter != vec_var_def.end(); ++iter)
		{
			(*iter)->Dump();
		}
		--depth_dump;
		if (debug)
			print_dump("VarDecl", DEBUG_END);
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
		if (debug)
			print_dump("ConstDef", DEBUG_BEGIN);
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
		if (debug)
			print_dump("ConstDef", DEBUG_END);
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