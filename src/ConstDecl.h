#include "Base.h"
#include "Debug.h"
#include <memory>
#include <vector>
#include <cassert>

extern bool is_calculating_const_exp;

class ConstDeclAST : public BaseAST
{
public:
    std::unique_ptr<BaseTypeAST> b_type;
    std::vector<std::unique_ptr<BaseAST>> vec_const_def;
    void Dump(int ctl = 0) override;
};