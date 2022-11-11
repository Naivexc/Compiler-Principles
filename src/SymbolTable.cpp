#include "SymbolTable.h"

SymbolTable *SymbolTable::AddSymbolTable()
{
    std::unique_ptr<SymbolTable> new_symbol_table(new SymbolTable);
    new_symbol_table->father_symbol_table = this;
    vec_child_symbol_table.push_back(std::move(new_symbol_table));
    return vec_child_symbol_table.back().get();
}
auto SymbolTable::find(std::string ident) -> std::map<std::string, std::unique_ptr<Symbol>>::iterator
{
    auto iter = map_symbol_table.find(ident);
    if (iter == map_symbol_table.end())
    {
        if (father_symbol_table == NULL)
            return map_symbol_table.end();
        else
            return father_symbol_table->find(ident);
    }
    else
    {
        return iter;
    }
}
auto SymbolTable::insert(std::string __ident__, Variate __variate__)
    -> std::pair<std::map<std::string, std::unique_ptr<Symbol>>::iterator, bool>
{
    return map_symbol_table.insert(make_pair(__ident__, std::unique_ptr<Symbol>(new Symbol(__variate__))));
}
auto SymbolTable::insert(std::string __ident__, Function __function__)
    -> std::pair<std::map<std::string, std::unique_ptr<Symbol>>::iterator, bool>
{
    return map_symbol_table.insert(make_pair(__ident__, std::unique_ptr<Symbol>(new Symbol(__function__))));
}
void SymbolTable::print_symbol_table()
{
    auto iter = map_symbol_table.begin();
    if (iter == map_symbol_table.end())
    {
        printf("empty\n");
        return;
    }
    while (iter != map_symbol_table.end())
    {
        printf("%s: ", iter->first.c_str());
        if (iter->second->tag == 0)
            printf("variate\n");
        else
            printf("function\n");
        ++iter;
    }
}