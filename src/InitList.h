#ifndef INIT_LIST_H
#define INIT_LIST_H
#include "CalData.h"
#include <vector>
#include <memory>

class InitList;

class InitListCase0
{
public:
    CalData data;
};

class InitListCase1
{
public:
    std::vector<std::unique_ptr<InitList>> vec_init_list;
};

class InitList
{
public:
    int32_t tag;
    union
    {
        InitListCase0 *init_list_case_0;
        InitListCase1 *init_list_case_1;
    } init_list_union;
    InitList(int32_t);
};
#endif