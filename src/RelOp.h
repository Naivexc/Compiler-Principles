#include "Debug.h"
#include "Base.h"
#include "SymbolTable.h"

class RelOpAST : public BaseOpAST
{
public:
    // tag 0:>|<;tag 1??>=|<=
    char op[3];
    void Dump(int ctl = 0) override;
};