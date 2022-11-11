#include "Base.h"

class FuncTypeAST : public BaseTypeAST
{
public:
    FuncTypeAST(std::string __type__) { type = __type__; };
    FuncTypeAST(){};
    void Dump(int ctl = 0) override;
};