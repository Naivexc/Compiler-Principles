#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>

class FuncFParamAST;

class Function
{
public:
    std::string ret_type;
    std::vector<FuncFParamAST *> params_type;
    Function();
    Function(std::string __ret_type__, std::vector<FuncFParamAST *> __params_type__);
};

#endif