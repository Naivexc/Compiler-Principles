#ifndef NUMBER_H
#define NUMBER_H

#include "Base.h"

class NumberAST : public BaseExpAST
{
public:
    std::string num;
    void Dump(int ctl = 0) override;
};

#endif