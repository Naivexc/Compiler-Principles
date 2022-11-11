#include "Base.h"
#include "Debug.h"

class NumberAST : public BaseExpAST
{
public:
    std::string num;
    void Dump(int ctl = 0) override;
};