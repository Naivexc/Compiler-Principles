#ifndef SYMBOL_H
#define SYMBOL_H

#include "Variate.h"
#include "Function.h"

struct Symbol
{
    union
    {
        Variate *var;
        Function *func;
    } data;
    int tag;
    Symbol(Variate __variate__);
    Symbol(Function __function__);
};

#endif