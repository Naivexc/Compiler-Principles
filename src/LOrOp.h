#include "Base.h"
#include "Debug.h"
#include "SymbolTable.h"

extern bool is_calculating_const_exp;
extern std::string k_str;
extern int32_t unused_koopa_count = 0;
extern SymbolTable *cur_symbol_table;

extern std::string itostr(int num);

class LOrOpAST : public BaseOpAST
{
public:
    char op[3];
    void Dump(int ctl = 0) override;
};