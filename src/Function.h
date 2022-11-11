#ifndef FUNCTION_H
#define FUNCTION_H

#include "FuncFParam.h"
#include <string>
#include <vector>
class Function
{
public:
    std::string ret_type;
    std::vector<FuncFParamAST *> params_type;
    Function()
    {
    }
    Function(std::string __ret_type__, std::vector<FuncFParamAST *> __params_type__)
    {
        ret_type = __ret_type__;
        params_type = __params_type__;
    }
};

#endif