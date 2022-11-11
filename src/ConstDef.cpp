#include "ConstDef.h"
std::string ConstDefCase1::return_type()
{
    std::string arr_type = "i32";
    int n = vec_const_exp.size();
    for (int i = 0; i < n; ++i)
    {
        arr_type = ("[" + arr_type + ", " + itostr(vec_const_exp[n - 1 - i]->data.val) + ']');
    }
    return arr_type;
}

ConstDefAST::ConstDefAST(int __tag__)
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
void ConstDefAST::InitGlobalArrayInKoopa(int dim)
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
void ConstDefAST::InitLocalArrayInKoopa(int dim, std::string str_src)
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
void ConstDefAST::Dump(int ctl = 0)
{
    PRINT_DUMP("ConstDef", DEBUG_BEGIN);
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
    PRINT_DUMP("ConstDef", DEBUG_END);
}
void ConstDefAST::Init()
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
int ConstDefAST::cur_init_index()
{
    int index = vec_cur_init_list.size() - 1;
    while (index >= 0 && vec_cur_init_list[index] == vec_begin_init_list[index])
        --index;
    if (index < 0)
        ++index;
    return index;
}
void ConstDefAST::Convert(ConstInitValAST *civ)
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