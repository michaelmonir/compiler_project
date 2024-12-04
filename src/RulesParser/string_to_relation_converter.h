#ifndef STRING_TO_RELATION_CONVERTER_H
#define STRING_TO_RELATION_CONVERTER_H

#include <map>
#include <string>
#include "structs.h"

Relation *get_relation_from_infix(string &input, map<string, Symbol *> input_symbol_map);

#endif // STRING_TO_RELATION_CONVERTER_H