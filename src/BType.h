#ifndef B_TYPE_H
#define B_TYPE_H

#include "Base.h"

class BTypeAST : public BaseTypeAST
{
public:
    BTypeAST(std::string __type__);
    BTypeAST();
    void Dump(int ctl = 0) override;
};

#endif