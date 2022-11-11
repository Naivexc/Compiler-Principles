/*
    Debug.h声明了一些debug用到的代码
*/
#ifndef DEBUG_H
#define DEBUG_H

/*
    一些可能用到的代码的声明
*/
#define DEBUG_BEGIN true
#define DEBUG_END false
extern void print_dump(std::string, bool);
extern int32_t depth_dump;

/*
    debug选项如下
*/

#define DEBUG_PRINT_DUMP

#ifdef DEBUG_PRINT_DUMP
extern void print_dump(std::string, bool);
#define PRINT_DUMP(ast_name, is_begin) print_dump(ast_name, is_begin);
#else
#define PRINT_DUMP(ast_name, is_begin)
#endif

#endif