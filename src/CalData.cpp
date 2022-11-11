#include "CalData.h"
#include <string>
#include <cstdlib>
#include <cstdio>
extern std::string k_str;
extern std::string itostr(int);
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