#include "Base.h"
#include "Debug.h"
#include "SymbolTable.h"

class AddOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl = 0) override;
};