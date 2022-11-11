#include "Base.h"
#include "Debug.h"

class BTypeAST : public BaseTypeAST
{
public:
    BTypeAST(std::string __type__);
    BTypeAST();
    void Dump(int ctl = 0) override;
};