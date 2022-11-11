#include "Base.h"

class VarDeclAST : public BaseAST
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::vector<std::unique_ptr<BaseAST>> vec_var_def;
    void Dump(int ctl = 0) override;
};