#include "BType.h"

BTypeAST::BTypeAST(std::string __type__) { type = __type__; };
BTypeAST::BTypeAST(){};
void BTypeAST::Dump(int ctl = 0)
{
    PRINT_DUMP("BType", DEBUG_BEGIN);
    if (type == "int")
    {
        // k_str += "i32";
    }
    PRINT_DUMP("BType", DEBUG_END);
}