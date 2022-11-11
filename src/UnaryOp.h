#include "Base.h"
#include "Debug.h"

extern bool is_calculating_const_exp;

class UnaryOpAST : public BaseOpAST
{
public:
    char op;
    void Dump(int ctl = 0) override
    {
        PRINT_DUMP("UnaryOp", DEBUG_BEGIN);
        printf("%c\n", op);
        if (op == '+')
        {
            result = rhs;
            goto END_OF_DUMP;
        }
        if (is_calculating_const_exp == false)
        {
            k_str += "    %";
            k_str += itostr(unused_koopa_count);
            k_str += " = ";
        }
        switch (op)
        {
        case ('-'):
        {
            if (is_calculating_const_exp == false)
                k_str += "sub";
            result.val = (-1) * rhs.val;
            break;
        }
        case ('!'):
        {
            if (is_calculating_const_exp == false)
                k_str += "eq";
            result.val = !rhs.val;
            break;
        }
        default:
        {
            assert(false);
        }
        }
        if (is_calculating_const_exp == false)
        {
            if (rhs.is_num == true)
            {

                k_str += " 0, ";
                k_str += itostr(rhs.val);
                k_str += '\n';
            }
            else
            {
                k_str += " 0, %";
                k_str += itostr(rhs.count);
                k_str += '\n';
            }
            result.is_num = false;
            result.count = unused_koopa_count;
            ++unused_koopa_count;
            std::cout << k_str << '\n';
            if (cur_symbol_table != symbol_table_tree.symbol_table_tree_root.get())
                cur_map_iter_for_func_space_needed->second.variate_needed += return_type_space("int");
        }
    END_OF_DUMP:
        PRINT_DUMP("UnaryOp", DEBUG_END);
    }
};