//
// Created by michael on 12/24/24.
//

#ifndef PARCERSTREAM_H
#define PARCERSTREAM_H

#include "ParseStructs.h"

void initialize_terminal_map_and_parse_table
    (map<ParseUnit, Token> _terminal_map,
     map<ParseUnit, map<Token, ParseTableItem>> _parse_table);
void initialize_stack(ParseUnit start_symbol);
void left_most_print_root();
void stream_process_input_token(Token token);

#endif //PARCERSTREAM_H
