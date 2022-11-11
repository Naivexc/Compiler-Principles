#include "LAndExp.h"

void LAndExpAST::Dump(int ctl)
{
    PRINT_DUMP("LAndExp", DEBUG_BEGIN);
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
    PRINT_DUMP("LAndExp", DEBUG_END);
}
LAndExpAST::LAndExpAST(int __tag__)
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