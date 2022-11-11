#include "Base.h"
#include "Debug.h"
#include "SymbolTable.h"

class ExpAST : public BaseExpAST
{
public:
    std::unique_ptr<BaseExpAST> l_or_exp;
    void Dump(int ctl = 0) override;
};