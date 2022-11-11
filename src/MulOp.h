#include "Base.h"
#include "Debug.h"
#include "SymbolTable.h"
#include <string>
#include <cassert>

extern std::string k_str;
extern bool is_calculating_const_exp;
extern int32_t unused_koopa_count = 0;
extern std::string itostr(int num);
extern SymbolTable *cur_symbol_table;

class MulOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl = 0) override;
};