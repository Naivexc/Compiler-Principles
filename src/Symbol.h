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
    Symbol(Variate __variate__)
    {
        tag = 0;
        data.var = new Variate();
        *data.var = __variate__;
    }
    Symbol(Function __function__)
    {
        tag = 1;
        data.func = new Function();
        *data.func = __function__;
    }
};

#endif