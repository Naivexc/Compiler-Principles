#include "Debug.h"
#include <cstdio>
#include <string>
int32_t depth_dump = 0;
void print_ident(uint32_t ident)
{
    for (uint32_t i = 0; i < ident; ++i)
        printf("  ");
}
void print_dump(std::string AST_name, bool is_begin)
{
    if (is_begin == true)
    {
        print_ident(depth_dump);
        printf("%sAST->Dump(): Begin\n", AST_name.c_str());
        print_ident(depth_dump);
        printf("{Depth: %d\n", depth_dump);
    }
    else
    {
        print_ident(depth_dump);
        printf("Depth: %d}\n", depth_dump);
        print_ident(depth_dump);
        printf("%sAST->Dump(): End\n", AST_name.c_str());
    }
}