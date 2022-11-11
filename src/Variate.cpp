#include "Variate.h"

Variate::Variate(bool __is_const__, int __val__, std::string __var_type__, int __sym_type__) : flag_is_const(__is_const__), val(__val__), var_type(__var_type__), sym_type(__sym_type__), koopa_var_count()
{
    if (__is_const__ == false)
    {
        koopa_var_count = unused_koopa_var_count;
    }
    else
    {
        koopa_var_count = -1;
    }
    dim = 0;
    for (int i = 0; i < var_type.length(); ++i)
        if (var_type[i] == '[' || var_type[i] == '*')
            ++dim;
};
Variate::Variate()
{
}