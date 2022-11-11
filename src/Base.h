/*
    Base.h定义了AST的基础节点类
*/
#ifndef BASE_H
#define BASE_H
#include "CalData.h"
#include "string"
#include "Debug.h"

#define IS_CONST true
#define IS_NOT_CONST false
#define STRONG 1
#define WEAK 0
#define GLOBAL 2
#define UN_DEF -1
#define GET_ARRAY_PTR 2
#define NORMAL 0
#define TAG_0 0
#define TAG_1 1
#define TAG_2 2
#define TAG_3 3
#define TAG_4 4
#define TAG_5 5
#define TAG_6 6
#define TAG_7 7
#define TAG_8 8
#define TAG_9 9
#define TAG_10 10
#define DIM_0 0
#define LINE(x)                 \
    {                           \
        printf("line:%d\n", x); \
    }

class BaseAST
{
public:
    int tag;
    virtual ~BaseAST() = default;

    virtual void Dump(int ctl = 0) = 0;
};

class BaseExpAST : public BaseAST
{
public:
    CalData data;
    virtual ~BaseExpAST() = default;
    virtual void Dump(int ctl = 0) = 0;
};

class BaseOpAST : public BaseAST
{
public:
    CalData lhs;
    CalData rhs;
    CalData result;
    virtual ~BaseOpAST() = default;
    virtual void Dump(int ctl = 0) = 0;
};

class BaseTypeAST : public BaseAST
{
public:
    std::string type;
    virtual ~BaseTypeAST() = default;
    virtual void Dump(int ctl = 0) = 0;
};

#endif