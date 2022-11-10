#include "ast.h"

std::string k_str = "decl @getint() : i32\n"
                    "decl @getch() : i32\n"
                    "decl @getarray(*i32) : i32\n"
                    "decl @putint(i32)\n"
                    "decl @putch(i32)\n"
                    "decl @putarray(i32, *i32)\n"
                    "decl @starttime()\n"
                    "decl @stoptime()\n\n";
void CalData::operator=(CalData rhs)
{
    this->is_num = rhs.is_num;
    this->val = rhs.val;
    this->count = rhs.count;
}
void CalData::print_data_to_k_str()
{
    if (is_num == true)
        k_str += itostr(val);
    else
        k_str += ("%" + itostr(count));
}
CalData::CalData() : is_num(true), val(0), count(0){};

InitList::InitList(int __tag__)
{
    tag = __tag__;
    switch (tag)
    {
    case (0):
        init_list_union.init_list_case_0 = new InitListCase0();
        init_list_union.init_list_case_0->data.is_num = true;
        init_list_union.init_list_case_0->data.val = 0;
        init_list_union.init_list_case_0->data.count = -1;
        break;
    case (1):
        init_list_union.init_list_case_1 = new InitListCase1();
        break;
    default:
        assert(false);
    }
    return;
}
