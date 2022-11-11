#include <memory>
#include <map>
#include <vector>
#include "Symbol.h"
class SymbolTable
{
public:
    SymbolTable *father_symbol_table; //在符号表树中的父节点
    std::map<std::string, std::unique_ptr<Symbol>> map_symbol_table;
    std::vector<std::unique_ptr<SymbolTable>> vec_child_symbol_table;
    SymbolTable *AddSymbolTable();
    auto find(std::string);
    auto insert(std::string, Variate);
    auto insert(std::string, Function);
    void print_symbol_table();
};

/*
    一个符号表树类,实际上只有一个指向根节点的指针......
*/
class SymbolTableTree
{
public:
    std::unique_ptr<SymbolTable> symbol_table_tree_root;
};