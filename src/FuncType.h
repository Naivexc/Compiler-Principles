#ifndef FUNC_TYPE_H
#define FUNC_TYPE_H

#include "Base.h"

extern std::string k_str;

class FuncTypeAST : public BaseTypeAST
{
public:
    FuncTypeAST(std::string __type__) { type = __type__; };
    FuncTypeAST(){};
    void Dump(int ctl = 0) override;
};

#endif