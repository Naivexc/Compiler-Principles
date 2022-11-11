#include "InitList.h"
#include <cassert>

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
}