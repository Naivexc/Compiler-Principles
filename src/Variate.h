#ifndef VARIATE_H
#define VARIATE_H

#include <string>
extern int32_t unused_koopa_var_count;
class Variate
{
public:
    bool flag_is_const;
    int val;
    std::string var_type; //"int"
    int sym_type;         // STRONG WEAK
    int koopa_var_count;
    int dim;
    Variate(bool __is_const__, int __val__, std::string __var_type__, int __sym_type__);
    Variate();
};

#endif