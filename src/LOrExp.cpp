#include "LOrExp.h"

void LOrExpAST::Dump(int ctl = 0)
{
    PRINT_DUMP("LOrExp", DEBUG_BEGIN);
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
    PRINT_DUMP("LOrExp", DEBUG_END);
}
LOrExpAST::LOrExpAST(int __tag__)
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