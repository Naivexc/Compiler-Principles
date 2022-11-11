#include "Base.h"
#include "FuncType.h"
#include "FuncFParams.h"

class FuncDefAST : public BaseAST
{
public:
    std::unique_ptr<BaseTypeAST> func_type;
    std::string ident;
    std::unique_ptr<BaseAST> block;
    std::unique_ptr<BaseAST> func_f_params;
    void Dump(int ctl = 0) override;
};