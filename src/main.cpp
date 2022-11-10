#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include "ast.h"
#include "koopa.h"
#include "riscv.h"
#include <map>
#include <queue>
extern FILE *yyout;
int label_j_count = 0;

#pragma region EXTERN
extern FILE *yyin;
extern int yyparse(std::unique_ptr<BaseAST> &ast);
extern koopa_error_code_t koopa_parse_from_string(const char *str, koopa_program_t *program);
extern bool str_is_same(const char *s1, const char *s2);
extern int return_size_of_value(const koopa_raw_type_kind *const __kind__);
std::string k_str = "decl @getint() : i32\n"
					"decl @getch() : i32\n"
					"decl @getarray(*i32) : i32\n"
					"decl @putint(i32)\n"
					"decl @putch(i32)\n"
					"decl @putarray(i32, *i32)\n"
					"decl @starttime()\n"
					"decl @stoptime()\n\n";
static std::string data_str = "";
static std::string text_str = "";
static int unused_koopa_count = 0;
static int space_needed = 0;
static bool debug = false;
static int depth_dump = 0;
static int depth_visit = 0;
static int sp_dev = 0;
static std::map<uint64_t, int> ins_map;
static std::queue<std::string> global_var_name_queue;
static std::map<uint64_t, std::string> global_var_name_map;
static void *slice_ptr;
static SymbolTableTree symbol_table_tree;
static bool is_calculating_const_exp;
static bool cur_block_is_entry;
static bool last_ins_is_ret;
static int unused_koopa_var_count;
static int unused_koopa_label_count;
static int unused_riscv_label_count;
SymbolTable *cur_symbol_table;
std::string op_name[] = {"NOT_EQ", "EQ", "GT", "LT", "GE", "LE", "ADD", "SUB", "MUL", "DIV", "MOD", "AND", "OR", "XOR", "SHL", "SHR", "SAR"};
std::string reg_for_func_params[8] = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"};
ConstInitValAST ConstDefAST::zero_init = ConstInitValAST(TAG_0);
InitValAST VarDefAST::zero_init = InitValAST(TAG_0);
#pragma endregion EXTERN
void global_var_init()
{
	cur_block_is_entry = false;
	unused_koopa_var_count = 1;
	unused_koopa_count = 0;
	unused_koopa_label_count = 0;
	unused_riscv_label_count = 0;
	is_calculating_const_exp = false;
	last_ins_is_ret = false;
	cur_symbol_table = symbol_table_tree.symbol_table_tree_root.get();
	cur_symbol_table->insert("@getint", Function("int", std::vector<FuncFParamAST *>{}));
	cur_symbol_table->insert("@getch", Function("int", std::vector<FuncFParamAST *>{}));
	cur_symbol_table->insert("@getarray", Function("int", std::vector<FuncFParamAST *>{new FuncFParamAST(TAG_1, "int")}));
	cur_symbol_table->insert("@putint", Function("void", std::vector<FuncFParamAST *>{new FuncFParamAST(TAG_0, "int")}));
	cur_symbol_table->insert("@putch", Function("void", std::vector<FuncFParamAST *>{new FuncFParamAST(TAG_0, "int")}));
	cur_symbol_table->insert("@putarray", Function("void", std::vector<FuncFParamAST *>{new FuncFParamAST(TAG_0, "int"), new FuncFParamAST(TAG_1, "int")}));
	cur_symbol_table->insert("@starttime", Function("void", std::vector<FuncFParamAST *>{}));
	cur_symbol_table->insert("@stoptime", Function("void", std::vector<FuncFParamAST *>{}));
}
void print_ident(int ident)
{
	for (int i = 0; i < ident; ++i)
		printf("  ");
}
void print_dump(std::string AST_name, bool is_begin)
{
	if (is_begin == true)
	{
		print_ident(depth_dump);
		printf("%sAST->Dump(): Begin\n", AST_name.c_str());
		print_ident(depth_dump);
		printf("{Depth: %d\n", depth_dump);
	}
	else
	{
		print_ident(depth_dump);
		printf("Depth: %d}\n", depth_dump);
		print_ident(depth_dump);
		printf("%sAST->Dump(): End\n", AST_name.c_str());
	}
}
std::string itostr(int num)
{
	std::string str = "";
	if (num == 0)
	{
		str += '0';
		return str;
	}
	if (num < 0)
	{
		str += '-';
		num *= -1;
	}
	std::stack<char> s;
	while (num > 0)
	{
		s.push(num % 10 + '0');
		num /= 10;
	}
	while (!s.empty())
	{
		str += s.top();
		s.pop();
	}
	return str;
}
int strtoi(std::string str)
{
	int s = 1;
	int i = 0;
	int val = 0;
	if (str[0] == '-')
	{
		s = -1;
		i = 1;
	}
	while (str[i] != '\0')
	{
		val *= 10;
		val += (str[i] - '0');
		++i;
	}
	val *= s;
	return val;
}
int return_type_space(std::string type)
{
	if (type == "int")
		return 4;
	assert(false);
}
bool str_is_same(const char *s1, const char *s2)
{
	int pos = 0;
	while (s1[pos] != '\0' && s2[pos] != '\0')
	{
		if (s1[pos] != s2[pos])
			return false;
		++pos;
	}
	if (s1[pos] == '\0' && s2[pos] == '\0')
		return true;
	else
		return false;
}
std::map<std::string, SpaceNeeded> map_space_needed_for_function;
std::map<std::string, SpaceNeeded>::iterator cur_map_iter_for_func_space_needed;
int max_int(int x, int y)
{
	return x >= y ? x : y;
}
void ConstInitValInit(std::vector<std::unique_ptr<BaseExpAST>>::iterator iter_begin, std::vector<std::unique_ptr<BaseExpAST>>::iterator iter_end, InitList *civ)
{
	int arr_len = (*iter_begin)->data.val;
	++iter_begin;
	if (iter_begin == iter_end)
	{
		for (int i = 0; i < arr_len; ++i)
			civ->init_list_union.init_list_case_1->vec_init_list.push_back(std::unique_ptr<InitList>(new InitList(TAG_0)));
		return;
	}
	else
	{
		for (int i = 0; i < arr_len; ++i)
		{
			civ->init_list_union.init_list_case_1->vec_init_list.push_back(std::unique_ptr<InitList>(new InitList(TAG_1)));
			ConstInitValInit(iter_begin, iter_end, civ->init_list_union.init_list_case_1->vec_init_list.back().get());
		}
		return;
	}
}
void generate_raw_program(koopa_raw_program_t &raw, koopa_raw_program_builder_t &builder)
{
	koopa_program_t program;
	koopa_error_code_t ret = koopa_parse_from_string(k_str.c_str(), &program);
	assert(ret == KOOPA_EC_SUCCESS); // ȷ������ʱû�г���
	// ����һ�� raw program builder, �������� raw program
	builder = koopa_new_raw_program_builder();
	// �� Koopa IR ����ת��Ϊ raw program
	raw = koopa_build_raw_program(builder, program);
	// �ͷ� Koopa IR ����ռ�õ��ڴ�
	koopa_delete_program(program);
}
#pragma region Visit

#pragma region program

void Visit(const koopa_raw_program_t &program)
{
	// ִ��һЩ�����ı�Ҫ����

	// ��������ȫ�ֱ���
	++depth_visit;
	Visit(program.values);
	// �������к���
	--depth_visit;
	++depth_visit;
	Visit(program.funcs);
	--depth_visit;
}

#pragma endregion program

#pragma region slice

void Visit(const koopa_raw_slice_t &slice)
{
	for (size_t i = 0; i < slice.len; ++i)
	{
		auto ptr = slice.buffer[i];
		slice_ptr = const_cast<void *>(ptr);
		// ���� slice �� kind ������ ptr ��������Ԫ��
		switch (slice.kind)
		{
		case KOOPA_RSIK_FUNCTION:
			// ���ʺ���
			++depth_visit;
			Visit(reinterpret_cast<koopa_raw_function_t>(ptr));
			--depth_visit;
			break;
		case KOOPA_RSIK_BASIC_BLOCK:
			// ���ʻ�����
			++depth_visit;
			Visit(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
			--depth_visit;
			break;
		case KOOPA_RSIK_VALUE:
			// ����ָ��
			++depth_visit;
			Visit(reinterpret_cast<koopa_raw_value_t>(ptr));
			--depth_visit;
			break;
		default:
			// ������ʱ����������������, ���ǲ��������κδ���
			assert(false);
		}
	}
}

#pragma endregion slice

#pragma region func

void Visit(const koopa_raw_function_t &func)
{
	if (func->bbs.len == 0)
		return;

	cur_map_iter_for_func_space_needed = map_space_needed_for_function.find(func->name);
	sp_dev = cur_map_iter_for_func_space_needed->second.params_needed; //����ÿ������ʱ�Ƚ�ƫ������0
	if (cur_map_iter_for_func_space_needed == map_space_needed_for_function.end())
		assert(false);
	fprintf(yyout, "  .text\n");
	fprintf(yyout, "  .globl %s\n", func->name + 1); //+1����Ϊ��һ���ַ���@
	fprintf(yyout, "%s:\n", func->name + 1);
	if (cur_map_iter_for_func_space_needed->second.total_needed > 0)
		ADDI("sp", "sp", -cur_map_iter_for_func_space_needed->second.total_needed);
	if (cur_map_iter_for_func_space_needed->second.return_address_needed > 0)
		SW("ra", cur_map_iter_for_func_space_needed->second.total_needed - 4, "sp");
	// �������л�����
	++depth_visit;
	Visit(func->bbs);
	--depth_visit;
	fprintf(yyout, "\n");
}

#pragma endregion func

#pragma region basic block

void Visit(const koopa_raw_basic_block_t &bb)
{
	if (str_is_same(bb->name, "%entry") == false)
		fprintf(yyout, "%s:\n", bb->name + 1);
	++depth_visit;
	Visit(bb->insts);
	--depth_visit;
}

#pragma endregion basic block

#pragma region instruction

void Visit(const koopa_raw_value_t &value)
{
	// ����ָ�������жϺ�����Ҫ��η���
	const auto &kind = value->kind;
	void *cur_slice_ptr = const_cast<void *>(slice_ptr);
	switch (kind.tag)
	{
	case (KOOPA_RVT_RETURN):
	{
		// ���� return ָ��
		++depth_visit;
		Visit(kind.data.ret);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_INTEGER):
		// ���� integer ָ��
		{
			++depth_visit;
			Visit(kind.data.integer);
			--depth_visit;
			break;
		}
	case (KOOPA_RVT_BINARY):
	{
		++depth_visit;
		Visit(kind.data.binary);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_STORE):
	{
		++depth_visit;
		Visit(kind.data.store);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_LOAD):
	{
		++depth_visit;
		Visit(kind.data.load);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_ALLOC):
	{
		switch (value->ty->tag)
		{
		case (KOOPA_RTT_POINTER):
		{
			// pointer
			switch (value->ty->data.pointer.base->tag)
			{
			case (KOOPA_RTT_INT32):
			{
				ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
				sp_dev += 4;
				break;
			}
			case (KOOPA_RTT_ARRAY):
			{
				ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
				sp_dev += return_size_of_value(value->ty->data.pointer.base);
				break;
			}
			case (KOOPA_RTT_POINTER):
			{
				ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
				sp_dev += 4;
				break;
			}
			default:
			{
				printf("%d\n", value->ty->data.pointer.base->tag);
				assert(false);
				break;
			}
			}
			break;
		}
		default:
		{
			printf("%d\n", value->ty->tag);
			assert(false);
			break;
		}
		}
		break;
	}
	case (KOOPA_RVT_BRANCH):
	{
		++depth_visit;
		Visit(kind.data.branch);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_JUMP):
	{
		++depth_visit;
		Visit(kind.data.jump);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_CALL):
	{
		++depth_visit;
		Visit(kind.data.call);
		if (value->used_by.len > 0)
		{
			SW("a0", sp_dev, "sp");
			ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
			sp_dev += 4;
		}
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_GLOBAL_ALLOC):
	{
		++depth_visit;
		Visit(kind.data.global_alloc);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_GET_ELEM_PTR):
	{
		++depth_visit;
		Visit(kind.data.get_elem_ptr);
		--depth_visit;
		break;
	}
	case (KOOPA_RVT_GET_PTR):
	{
		++depth_visit;
		Visit(kind.data.get_ptr);
		--depth_visit;
		break;
	}
	default:
	{
		// ����������ʱ������
		printf("%d\n", kind.tag);
		assert(false);
	}
	}
}

#pragma endregion instruction

#pragma region ins_integer

void Visit(koopa_raw_integer_t integer)
{
}

#pragma endregion ins_integer

#pragma region ins_ret

void Visit(koopa_raw_return_t ret)
{

	if (ret.value != NULL)
	{
		switch (ret.value->kind.tag)
		{
		case KOOPA_RVT_INTEGER:
		{
			DEBUG("ret int");
			LI("a0", ret.value->kind.data.integer.value);
			break;
		}
		case KOOPA_RVT_LOAD:
		{
			auto iter_ret = ins_map.find(uint64_t(ret.value));
			if (iter_ret == ins_map.end())
				assert(false);
			DEBUG("ret load");
			LW("a0", iter_ret->second, "sp");
			break;
		}
		case KOOPA_RVT_BINARY:
		{
			auto iter_ret = ins_map.find(uint64_t(ret.value));
			if (iter_ret == ins_map.end())
				assert(false);
			DEBUG("ret binary");
			LW("a0", iter_ret->second, "sp");
			break;
		}
		case KOOPA_RVT_CALL:
		{
			auto iter_ret = ins_map.find(uint64_t(ret.value));
			if (iter_ret == ins_map.end())
				assert(false);
			DEBUG("ret call");
			LW("a0", iter_ret->second, "sp");
			break;
		}
		default:
			assert(false);
		}
	}
	if (cur_map_iter_for_func_space_needed->second.return_address_needed > 0)
		LW("ra", cur_map_iter_for_func_space_needed->second.total_needed - 4, "sp");
	if (cur_map_iter_for_func_space_needed->second.total_needed > 0)
		ADDI("sp", "sp", cur_map_iter_for_func_space_needed->second.total_needed);
	RET
}

#pragma endregion ins_ret

#pragma region ins_binary

void Visit(koopa_raw_binary_t binary)
{
	void *cur_slice_ptr = const_cast<void *>(slice_ptr); //��Visit���������ʵ�slice.buffer�еĵ�ַ

	auto lhs = binary.lhs; //��������
	auto rhs = binary.rhs; //��������
	int lhs_dev = -1, rhs_dev = -1;
	DEBUG("binary");
	switch (lhs->kind.tag)
	{
	case KOOPA_RVT_INTEGER:
	{
		LI("t0", lhs->kind.data.integer.value);
		break;
	}
	case KOOPA_RVT_BINARY:
	{
		auto iter_lhs = ins_map.find(uint64_t(lhs));
		if (iter_lhs == ins_map.end())
			assert(false);
		lhs_dev = iter_lhs->second;
		LW("t0", lhs_dev, "sp");
		break;
	}
	case KOOPA_RVT_LOAD:
	{
		auto iter_lhs = ins_map.find(uint64_t(lhs));
		if (iter_lhs == ins_map.end())
			assert(false);
		lhs_dev = iter_lhs->second;
		// lw t0, 0(sp)
		LW("t0", lhs_dev, "sp");
		break;
	}
	case KOOPA_RVT_GLOBAL_ALLOC:
	{
		LA("t0", lhs->kind.data.global_alloc.init->name + 1);
		LW("t0", 0, "t0");
		break;
	}
	case KOOPA_RVT_CALL:
	{
		auto iter_lhs = ins_map.find(uint64_t(lhs));
		if (iter_lhs == ins_map.end())
			assert(false);
		lhs_dev = iter_lhs->second;
		LW("t0", lhs_dev, "sp");
		break;
	}
	default:
		printf("%d\n", lhs->kind.tag);
		assert(false);
		break;
	}
	switch (rhs->kind.tag)
	{
	case KOOPA_RVT_INTEGER:
	{
		LI("t1", rhs->kind.data.integer.value);
		break;
	}
	case KOOPA_RVT_BINARY:
	{
		auto iter_rhs = ins_map.find(uint64_t(rhs));
		if (iter_rhs == ins_map.end())
			assert(false);
		rhs_dev = iter_rhs->second;
		LW("t1", rhs_dev, "sp");
		break;
	}
	case KOOPA_RVT_LOAD:
	{
		auto iter_rhs = ins_map.find(uint64_t(rhs));
		if (iter_rhs == ins_map.end())
			assert(false);
		rhs_dev = iter_rhs->second;
		LW("t1", rhs_dev, "sp");
		break;
	}
	case KOOPA_RVT_GLOBAL_ALLOC:
	{
		LA("t1", rhs->kind.data.global_alloc.init->name + 1);
		LW("t1", 0, "t1");
		break;
	}
	case KOOPA_RVT_CALL:
	{
		auto iter_rhs = ins_map.find(uint64_t(rhs));
		if (iter_rhs == ins_map.end())
			assert(false);
		rhs_dev = iter_rhs->second;
		LW("t1", rhs_dev, "sp");
		break;
	}
	default:
		printf("%d\n", rhs->kind.tag);
		assert(false);
		break;
	}
	switch (binary.op)
	{
	case KOOPA_RBO_NOT_EQ:
		XOR("t0", "t0", "t1");
		SNEZ("t0", "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	case KOOPA_RBO_EQ:
		XOR("t0", "t0", "t1");
		SEQZ("t0", "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	case KOOPA_RBO_GT:
		SLT("t0", "t1", "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	case KOOPA_RBO_LT:
		SLT("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	case KOOPA_RBO_GE:
		SLT("t0", "t0", "t1");
		XORI("t0", "t0", 1);
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	case KOOPA_RBO_LE:
		SLT("t0", "t1", "t0");
		XORI("t0", "t0", 1);
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	case KOOPA_RBO_ADD:
		ADD("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	case KOOPA_RBO_SUB:
		// sub   t1, x0, t0
		SUB("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	case KOOPA_RBO_MUL:
		MUL("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	case KOOPA_RBO_DIV:
		DIV("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	case KOOPA_RBO_MOD:
		REM("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	case KOOPA_RBO_AND:
		SNEZ("t0", "t0");
		SNEZ("t1", "t1");
		AND("t0", "t0", "t1");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	case KOOPA_RBO_OR:
		OR("t0", "t0", "t1");
		SNEZ("t0", "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(uint64_t(cur_slice_ptr), sp_dev));
		sp_dev += 4;
		//++temp_reg_num;
		break;
	default:
		assert(false);
	}
}

#pragma endregion ins_binary

#pragma region ins_store

void Visit(koopa_raw_store_t store)
{
	DEBUG("store");
	switch (store.value->kind.tag)
	{
	case KOOPA_RVT_INTEGER:
	{
		auto iter_dest = ins_map.find(uint64_t(store.dest));
		LI("t0", store.value->kind.data.integer.value);
		if (iter_dest == ins_map.end())
		{
			auto iter_global_var = global_var_name_map.find(uint64_t(store.dest));
			if (iter_global_var == global_var_name_map.end())
				assert(false);
			LA("t1", iter_global_var->second.c_str() + 1);
			SW("t0", 0, "t1");
		}
		else
		{
			if (store.dest->kind.tag == KOOPA_RVT_ALLOC)
			{
				SW("t0", iter_dest->second, "sp");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_ELEM_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW("t0", 0, "t1");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW("t0", 0, "t1")
			}
			else
			{
				printf("%d\n", store.dest->kind.tag);
				assert(false);
			}
		}
		break;
	}
	case KOOPA_RVT_FUNC_ARG_REF: // continue
	{
		auto iter_dest = ins_map.find(uint64_t(store.dest));
		if (store.value->kind.data.func_arg_ref.index <= 7)
		{
			if (iter_dest == ins_map.end())
				assert(false);
			if (store.dest->kind.tag == KOOPA_RVT_ALLOC)
			{
				SW(reg_for_func_params[store.value->kind.data.func_arg_ref.index].c_str(), iter_dest->second, "sp");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_ELEM_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW(reg_for_func_params[store.value->kind.data.func_arg_ref.index].c_str(), 0, "t1");
			}
			else
				assert(false);
			break;
		}
		else
		{
			LW("t0", cur_map_iter_for_func_space_needed->second.total_needed + (int(store.value->kind.data.func_arg_ref.index) - 8) * 4, "sp");

			if (store.dest->kind.tag == KOOPA_RVT_ALLOC)
			{
				SW("t0", iter_dest->second, "sp");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_ELEM_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW("t0", 0, "t1");
			}
			else
				assert(false);
			break;
		}
	}
	case KOOPA_RVT_LOAD:
	{
		//=binary
	}
	case KOOPA_RVT_BINARY:
	{
		auto iter_value = ins_map.find(uint64_t(store.value));
		auto iter_dest = ins_map.find(uint64_t(store.dest));
		if (iter_value == ins_map.end())
		{
			auto iter_global_var = global_var_name_map.find(uint64_t(store.value));
			if (iter_global_var == global_var_name_map.end())
				assert(false);
			LA("t0", iter_global_var->second.c_str() + 1);
			LW("t0", 0, "t0");
		}
		else
		{
			LW("t0", iter_value->second, "sp");
		}
		if (iter_dest == ins_map.end())
		{
			auto iter_global_var = global_var_name_map.find(uint64_t(store.dest));
			if (iter_global_var == global_var_name_map.end())
				assert(false);
			LA("t1", iter_global_var->second.c_str() + 1);
			SW("t0", 0, "t1");
		}
		else
		{
			if (store.dest->kind.tag == KOOPA_RVT_ALLOC)
			{
				SW("t0", iter_dest->second, "sp");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_ELEM_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW("t0", 0, "t1");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW("t0", 0, "t1");
			}
			else
				assert(false);
		}
		break;
	}
	case KOOPA_RVT_CALL:
	{
		auto iter_value = ins_map.find(uint64_t(store.value));
		auto iter_dest = ins_map.find(uint64_t(store.dest));
		if (iter_value == ins_map.end())
		{
			auto iter_global_var = global_var_name_map.find(uint64_t(store.value));
			if (iter_global_var == global_var_name_map.end())
				assert(false);
			LA("t0", iter_global_var->second.c_str() + 1);
			LW("t0", 0, "t0");
		}
		else
		{
			LW("t0", iter_value->second, "sp");
		}
		if (iter_dest == ins_map.end())
		{
			auto iter_global_var = global_var_name_map.find(uint64_t(store.dest));
			if (iter_global_var == global_var_name_map.end())
				assert(false);
			LA("t1", iter_global_var->second.c_str() + 1);
			SW("t0", 0, "t1");
		}
		else
		{
			if (store.dest->kind.tag == KOOPA_RVT_ALLOC)
			{
				SW("t0", iter_dest->second, "sp");
			}
			else if (store.dest->kind.tag == KOOPA_RVT_GET_ELEM_PTR)
			{
				LW("t1", iter_dest->second, "sp");
				SW("t0", 0, "t1");
			}
			else
				assert(false);
		}
		break;
	}
	default:
	{
		printf("%d\n", store.value->kind.tag);
		assert(false);
		break;
	}
	}
}

#pragma endregion ins_store

#pragma region ins_load

void Visit(koopa_raw_load_t load)
{
	DEBUG("load");
	switch (load.src->kind.tag)
	{
	case KOOPA_RVT_ALLOC:
	{

		auto iter_src = ins_map.find(uint64_t(load.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	}
	case KOOPA_RVT_GLOBAL_ALLOC:
	{
		auto iter_src = global_var_name_map.find(uint64_t(load.src));
		if (iter_src == global_var_name_map.end())
			assert(false);
		LA("t0", iter_src->second.c_str() + 1);
		LW("t0", 0, "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	}
	case (KOOPA_RVT_GET_PTR):
	{
		auto iter_src = ins_map.find(uint64_t(load.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		LW("t0", 0, "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	}
	case (KOOPA_RVT_GET_ELEM_PTR):
	{
		auto iter_src = ins_map.find(uint64_t(load.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		LW("t0", 0, "t0");
		SW("t0", sp_dev, "sp");
		ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
		sp_dev += 4;
		break;
	}
	default:
	{
		printf("%d\n", load.src->kind.tag);
		assert(false);
		break;
	}
	}
}

#pragma endregion ins_load

#pragma region ins_branch

void Visit(koopa_raw_branch_t branch)
{
	DEBUG("branch");
	if (branch.cond->kind.tag == KOOPA_RVT_INTEGER)
	{
		LI("t0", branch.cond->kind.data.integer.value);
		BNEZ("t0", branch.true_bb->name + 1);
		J(branch.false_bb->name + 1);
		return;
	}
	auto iter_cond = ins_map.find(uint64_t(branch.cond));
	if (iter_cond == ins_map.end())
		assert(false);
	LW("t0", iter_cond->second, "sp");
	BNEZ("t0", branch.true_bb->name + 1);
	J(branch.false_bb->name + 1);
}

#pragma endregion ins_branch

#pragma region ins_jump

void Visit(koopa_raw_jump_t jump)
{
	DEBUG("jump");
	J(jump.target->name + 1);
}

#pragma endregion ins_jump

#pragma region ins_call

void Visit(koopa_raw_call_t call)
{
	DEBUG("call");
	for (int i = 0; i < 8 && i < call.args.len; ++i)
	{
		if (call.args.kind != KOOPA_RSIK_VALUE)
			assert(false);
		switch (reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i])->kind.tag)
		{
		case KOOPA_RVT_INTEGER:
		{
			LI(reg_for_func_params[i].c_str(), reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i])->kind.data.integer.value);
			break;
		}
		case KOOPA_RVT_BINARY:
		{
			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
			{
				auto iter_global_var = global_var_name_map.find(uint64_t(call.args.buffer[i]));
				if (iter_global_var == global_var_name_map.end())
					assert(false);
				LA(reg_for_func_params[i].c_str(), iter_global_var->second.c_str() + 1);
				LW(reg_for_func_params[i].c_str(), 0, reg_for_func_params[i].c_str());
			}
			else
			{
				LW(reg_for_func_params[i].c_str(), iter_ret->second, "sp");
			}
			break;
		}
		case KOOPA_RVT_CALL:
		{
			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
			{
				auto iter_global_var = global_var_name_map.find(uint64_t(call.args.buffer[i]));
				if (iter_global_var == global_var_name_map.end())
					assert(false);
				LA(reg_for_func_params[i].c_str(), iter_global_var->second.c_str() + 1);
				LW(reg_for_func_params[i].c_str(), 0, reg_for_func_params[i].c_str());
			}
			else
			{
				LW(reg_for_func_params[i].c_str(), iter_ret->second, "sp");
			}
			break;
		}
		case KOOPA_RVT_LOAD:
		{

			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
			{
				auto iter_global_var = global_var_name_map.find(uint64_t(call.args.buffer[i]));
				if (iter_global_var == global_var_name_map.end())
					assert(false);
				LA(reg_for_func_params[i].c_str(), iter_global_var->second.c_str() + 1);
				LW(reg_for_func_params[i].c_str(), 0, reg_for_func_params[i].c_str());
			}
			else
			{
				LW(reg_for_func_params[i].c_str(), iter_ret->second, "sp");
			}
			break;
		}
		case KOOPA_RVT_GET_ELEM_PTR:
		{

			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
			{
				auto iter_global_var = global_var_name_map.find(uint64_t(call.args.buffer[i]));
				if (iter_global_var == global_var_name_map.end())
					assert(false);
				LA(reg_for_func_params[i].c_str(), iter_global_var->second.c_str() + 1);
				LW(reg_for_func_params[i].c_str(), 0, reg_for_func_params[i].c_str());
			}
			else
			{
				LW(reg_for_func_params[i].c_str(), iter_ret->second, "sp");
				// LW(reg_for_func_params[i].c_str(), 0, reg_for_func_params[i].c_str());
			}
			break;
		}
		default:
			printf("%d\n", reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i])->kind.tag);
			assert(false);
		}
	}
	for (int i = 8; i < call.args.len; ++i)
	{
		if (call.args.kind != KOOPA_RSIK_VALUE)
			assert(false);
		switch (reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i])->kind.tag)
		{
		case KOOPA_RVT_INTEGER:
		{
			LI("t0", reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i])->kind.data.integer.value);
			SW("t0", (i - 8) * 4, "sp");
			break;
		}
		case KOOPA_RVT_BINARY:
		{
			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
				assert(false);
			LW("t0", iter_ret->second, "sp");
			SW("t0", (i - 8) * 4, "sp");
			break;
		}
		case KOOPA_RVT_CALL:
		{
			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
				assert(false);
			LW("t0", iter_ret->second, "sp");
			SW("t0", (i - 8) * 4, "sp");
			break;
		}
		case KOOPA_RVT_LOAD:
		{
			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
				assert(false);
			LW("t0", iter_ret->second, "sp");
			SW("t0", (i - 8) * 4, "sp");
			break;
		}
		case KOOPA_RVT_GET_ELEM_PTR:
		{
			auto iter_ret = ins_map.find(uint64_t(call.args.buffer[i]));
			if (iter_ret == ins_map.end())
				assert(false);
			LW("t0", iter_ret->second, "sp");
			SW("t0", (i - 8) * 4, "sp");
			break;
		}
		default:
			assert(false);
		}
	}
	CALL(call.callee->name + 1);
}

#pragma endregion ins_call

int return_size_of_value(const koopa_raw_type_kind *const __kind__)
{
	int size = 1;
	koopa_raw_type_kind kind = *__kind__;
	if (kind.tag == KOOPA_RTT_INT32 || kind.tag == KOOPA_RTT_POINTER)
		size = 4;
	else if (kind.tag == KOOPA_RTT_UNIT || kind.tag == KOOPA_RTT_FUNCTION)
		assert(false);
	else if (kind.tag == KOOPA_RTT_ARRAY)
	{
		while (kind.tag == KOOPA_RTT_ARRAY)
		{
			size *= kind.data.array.len;
			kind = *kind.data.array.base;
		}
		if (kind.tag == KOOPA_RTT_INT32 || kind.tag == KOOPA_RTT_POINTER)
			size *= 4;
		else if (kind.tag == KOOPA_RTT_UNIT || kind.tag == KOOPA_RTT_FUNCTION)
			assert(false);
	}
	else
		assert(false);
	return size;
}

#pragma region global_alloc

void Visit(koopa_raw_aggregate_t aggregate) //��ʼ��
{
	for (int i = 0; i < aggregate.elems.len; ++i)
	{
		koopa_raw_value_data_t *value = reinterpret_cast<koopa_raw_value_data_t *>(const_cast<void *>(aggregate.elems.buffer[i]));
		switch (value->kind.tag)
		{
		case (KOOPA_RVT_INTEGER):
		{
			fprintf(yyout, "  .word %d\n", value->kind.data.integer.value);
			break;
		}
		case (KOOPA_RVT_AGGREGATE):
		{
			Visit(value->kind.data.aggregate);
			break;
		}
		default:
			printf("%d\n", value->kind.tag);
			assert(false);
		}
	}
}

void Visit(koopa_raw_global_alloc_t global_alloc)
{
	DEBUG("global alloc");
	void *cur_slice_ptr = const_cast<void *>(slice_ptr);
	std::string global_var_name = global_var_name_queue.front();
	global_var_name_queue.pop();
	global_var_name_map.insert(make_pair(uint64_t(cur_slice_ptr), global_var_name));
	switch (global_alloc.init->kind.tag)
	{
	case (KOOPA_RVT_ZERO_INIT):
	{
		fprintf(yyout, "  .data\n  .globl %s\n%s:\n  .zero %d\n\n", global_var_name.c_str() + 1, global_var_name.c_str() + 1, 4);
		break;
	}
	case (KOOPA_RVT_INTEGER):
	{
		fprintf(yyout, "  .data\n  .globl %s\n%s:\n  .word %d\n\n", global_var_name.c_str() + 1, global_var_name.c_str() + 1, global_alloc.init->kind.data.integer.value);
		break;
	}
	case (KOOPA_RVT_AGGREGATE):
	{
		fprintf(yyout, "  .data\n  .globl %s\n%s:\n", global_var_name.c_str() + 1, global_var_name.c_str() + 1);
		if (global_alloc.init->kind.data.aggregate.elems.kind == KOOPA_RSIK_VALUE)
		{
			Visit(global_alloc.init->kind.data.aggregate);
		}
		else
		{
			printf("%d\n", global_alloc.init->kind.data.aggregate.elems.kind);
			assert(false);
		}
		break;
	}
	default:
	{

		printf("%d\n", global_alloc.init->kind.tag);
		assert(false);
	}
	}
}

#pragma endregion global_alloc

#pragma region get_elem_ptr

void Visit(koopa_raw_get_ptr_t get_ptr)
{
	DEBUG("get ptr");
	if (get_ptr.src->ty->tag != KOOPA_RTT_POINTER)
		assert(false);
	int size_elem = return_size_of_value(get_ptr.src->ty->data.pointer.base);
	switch (get_ptr.src->kind.tag)
	{
	case KOOPA_RVT_ALLOC:
	{

		auto iter_src = ins_map.find(uint64_t(get_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		ADDI("t0", "sp", iter_src->second);
		break;
	}
	case KOOPA_RVT_GLOBAL_ALLOC:
	{
		auto iter_src = global_var_name_map.find(uint64_t(get_ptr.src));
		if (iter_src == global_var_name_map.end())
			assert(false);
		LA("t0", iter_src->second.c_str() + 1);
		break;
	}
	case (KOOPA_RVT_GET_ELEM_PTR):
	{
		auto iter_src = ins_map.find(uint64_t(get_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		break;
	}
	case (KOOPA_RVT_LOAD):
	{
		auto iter_src = ins_map.find(uint64_t(get_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		break;
	}
	default:
	{
		printf("%d\n", get_ptr.src->kind.tag);
		assert(false);
		break;
	}
	}
	switch (get_ptr.index->kind.tag)
	{
	case (KOOPA_RVT_INTEGER):
	{
		LI("t1", get_ptr.index->kind.data.integer.value);
		break;
	}
	case (KOOPA_RVT_LOAD):
	{
		auto iter_load = ins_map.find(uint64_t(get_ptr.index));
		if (iter_load == ins_map.end())
			assert(false);
		int load_dev = iter_load->second;
		LW("t1", load_dev, "sp");
		break;
	}
	case (KOOPA_RVT_BINARY):
	{
		auto iter_load = ins_map.find(uint64_t(get_ptr.index));
		if (iter_load == ins_map.end())
			assert(false);
		int load_dev = iter_load->second;
		LW("t1", load_dev, "sp");
		break;
	}
	default:
		printf("%d\n", get_ptr.index->kind.tag);
		assert(false);
	}
	LI("t2", size_elem);
	MUL("t1", "t1", "t2");
	ADD("t0", "t0", "t1");
	SW("t0", sp_dev, "sp");
	ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
	sp_dev += 4;
}

void Visit(koopa_raw_get_elem_ptr_t get_elem_ptr)
{
	DEBUG("get elem ptr");
	if (get_elem_ptr.src->ty->tag != KOOPA_RTT_POINTER)
		assert(false);
	if (get_elem_ptr.src->ty->data.pointer.base->tag != KOOPA_RTT_ARRAY)
	{
		printf("%d\n", get_elem_ptr.src->ty->data.pointer.base->tag);
		assert(false);
	}
	int size_elem = return_size_of_value(get_elem_ptr.src->ty->data.pointer.base->data.array.base);
	switch (get_elem_ptr.src->kind.tag)
	{
	case KOOPA_RVT_ALLOC:
	{

		auto iter_src = ins_map.find(uint64_t(get_elem_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		ADDI("t0", "sp", iter_src->second);
		break;
	}
	case KOOPA_RVT_GLOBAL_ALLOC:
	{
		auto iter_src = global_var_name_map.find(uint64_t(get_elem_ptr.src));
		if (iter_src == global_var_name_map.end())
			assert(false);
		LA("t0", iter_src->second.c_str() + 1);
		break;
	}
	case (KOOPA_RVT_LOAD):
	{
		auto iter_src = ins_map.find(uint64_t(get_elem_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		break;
	}
	case (KOOPA_RVT_GET_PTR):
	{
		auto iter_src = ins_map.find(uint64_t(get_elem_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		break;
	}
	case (KOOPA_RVT_GET_ELEM_PTR):
	{
		auto iter_src = ins_map.find(uint64_t(get_elem_ptr.src));
		if (iter_src == ins_map.end())
			assert(false);
		LW("t0", iter_src->second, "sp");
		break;
	}
	default:
	{
		printf("%d\n", get_elem_ptr.src->kind.tag);
		assert(false);
		break;
	}
	}
	switch (get_elem_ptr.index->kind.tag)
	{
	case (KOOPA_RVT_INTEGER):
	{
		LI("t1", get_elem_ptr.index->kind.data.integer.value);
		break;
	}
	case (KOOPA_RVT_LOAD):
	{
		auto iter_load = ins_map.find(uint64_t(get_elem_ptr.index));
		if (iter_load == ins_map.end())
			assert(false);
		int load_dev = iter_load->second;
		LW("t1", load_dev, "sp");
		break;
	}
	case (KOOPA_RVT_BINARY):
	{
		auto iter_load = ins_map.find(uint64_t(get_elem_ptr.index));
		if (iter_load == ins_map.end())
			assert(false);
		int load_dev = iter_load->second;
		LW("t1", load_dev, "sp");
		break;
	}
	default:
		printf("%d\n", get_elem_ptr.index->kind.tag);
		assert(false);
	}
	LI("t2", size_elem);
	MUL("t1", "t1", "t2");
	ADD("t0", "t0", "t1");
	SW("t0", sp_dev, "sp");
	ins_map.insert(std::make_pair(int64_t(slice_ptr), sp_dev));
	sp_dev += 4;
}

#pragma endregion get_elem_ptr

#pragma endregion Visit

int main(int argc, const char *argv[])
{
	// ���������в���. ���Խű�/����ƽ̨Ҫ����ı������ܽ������²���:
	// compiler ģʽ �����ļ� -o ����ļ�
	assert(argc == 5);
	auto mode = argv[1];
	auto input = argv[2];
	auto output = argv[4];
	// �������ļ�, ����ָ�� lexer �ڽ�����ʱ���ȡ����ļ�
	yyin = fopen(input, "r");
	yyout = fopen(output, "w");
	assert(yyin);
	// ���� parser ����, parser �������һ������ lexer ���������ļ���
	std::unique_ptr<BaseAST> ast;
	auto ret = yyparse(ast);
	assert(!ret);
	printf("parse accomplished\n");
	symbol_table_tree.symbol_table_tree_root = std::unique_ptr<SymbolTable>(new SymbolTable);
	global_var_init();
	ast->Dump();
	if (space_needed % 16 != 0)
		space_needed = (space_needed - space_needed % 16) + 16;
	printf("space_needed:%d\n", space_needed);
	// ��������õ��� AST, ��ʵ���Ǹ��ַ���
	// cout << k_str << '\n';//1
	if (yyout == NULL)
		assert(false);
	if (str_is_same(mode, "-koopa"))
		fprintf(yyout, "%s", k_str.c_str()); // test 1
	else if (str_is_same(mode, "-riscv"))
	{
		koopa_raw_program_builder_t builder;
		koopa_raw_program_t raw;
		generate_raw_program(raw, builder);
		Visit(raw);
		koopa_delete_raw_program_builder(builder);
	}
	else if (str_is_same(mode, "-perf"))
		return 0;
	else
		assert(false);
	return 0;
}