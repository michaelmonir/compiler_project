//
// Created by michael on 12/24/24.
//

#ifndef PARCERSTREAM_H
#define PARCERSTREAM_H

#include <set>

#include "ParseStructs.h"

// void initialize_terminal_map_and_parse_table
//     (map<ParseUnit, Token> _terminal_map,
//      map<ParseUnit, map<Token, ParseTableItem>> _parse_table);
void initialize_stack(ParseUnit start_symbol);
void left_most_print_root();
void stream_process_input_token(Token token);
void initialize_terminal_map_and_parse_table_adapt
    (set<string> terminal_set,
    std::map<std::string, std::map<std::string, std::pair<bool, std::vector<ParseUnit>>>> old_parse_unit);
void empty_stack_after_last_token();

#endif //PARCERSTREAM_H
