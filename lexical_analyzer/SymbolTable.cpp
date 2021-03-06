//
// Created by ahmed on 16/03/18.
//

#include "SymbolTable.h"

using namespace std;

map<string, string> Symbol_table::symbol_table;

void Symbol_table::add_identifier(string label, string value)
{
    symbol_table[label] = value;
}

bool Symbol_table::search_identifier(string label)
{
    return symbol_table[label] != "";
}