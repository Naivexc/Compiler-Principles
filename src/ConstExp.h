#include "Base.h"
#include "Debug.h"
#include "memory"

class ConstExpAST : public BaseExpAST
{
public:
    std::unique_ptr<BaseExpAST> exp;
    void Dump(int ctl = 0) override;
};