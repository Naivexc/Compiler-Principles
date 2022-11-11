#include "Symbol.h"
Symbol::Symbol(Variate __variate__)
{
    tag = 0;
    data.var = new Variate();
    *data.var = __variate__;
}
Symbol::Symbol(Function __function__)
{
    tag = 1;
    data.func = new Function();
    *data.func = __function__;
}