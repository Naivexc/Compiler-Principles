/*
    Base.h定义了AST的基础节点类
*/

#include "CalData.h"
#include "string"
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