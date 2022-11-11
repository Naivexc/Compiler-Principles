#include "Stmt.h"

void StmtAST::Dump(int ctl = 0)
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
StmtAST::StmtAST(int __tag__)
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