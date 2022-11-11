#include "Function.h"
Function::Function()
{
}
Function::Function(std::string __ret_type__, std::vector<FuncFParamAST *> __params_type__)
{
    ret_type = __ret_type__;
    params_type = __params_type__;
}