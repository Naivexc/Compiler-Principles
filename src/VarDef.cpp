#include "VarDef.h"

std::string VarDefCase1::return_type()
{
    std::string arr_type = "i32";
    int n = vec_const_exp.size();
    for (int i = 0; i < n; ++i)
    {
        arr_type = ("[" + arr_type + ", " + itostr(vec_const_exp[n - 1 - i]->data.val) + ']');
    }
    return arr_type;
}

VarDefAST::VarDefAST(int __tag__)
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
void VarDefAST::InitGlobalArrayInKoopa(int dim)
{
    switch (tag)
    {
    case (0):
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
void VarDefAST::InitLocalArrayInKoopa(int dim, std::string str_src)
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
void VarDefAST::Dump(int ctl)
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
void VarDefAST::Init()
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
int VarDefAST::cur_init_index()
{
    int index = vec_cur_init_list.size() - 1;
    while (index >= 0 && vec_cur_init_list[index] == vec_begin_init_list[index])
        --index;
    if (index < 0)
        ++index;
    return index;
}
void VarDefAST::Convert(InitValAST *iv)
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