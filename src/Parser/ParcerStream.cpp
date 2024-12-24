//
// Created by michael on 12/24/24.
//

#include "ParcerStream.h"
#include <stack>
#include "ParseStructs.h"

map<ParseUnit, Token> terminal_map;
map<Token, ParseUnit> inverse_terminal_map;
map<ParseUnit,map<Token, ParseTableItem>> parse_table;

stack<ParseUnit> parse_stack;

void stream_process_input_token(Token token) {
    if (parse_stack.empty()) {
        cout << "EMPTY STACK: Error at token " << token.token_name << endl;
        return;
    }
    ParseUnit top = parse_stack.top();

    if (top.type == ParseUnitType::TERMINAL) {
        if (terminal_map[top] != token) {
            cout << "Error at token " << token.token_name << endl;
        }
        parse_stack.pop();
    } else {
        if (parse_table[top].count(token) == 0) {
            cout << "Error at token " << token.token_name << endl;
            return;
        }
        ParseTableItem item = parse_table[top][token];

        if (item.isSync) {
            parse_stack.pop();
            cout << "Error at token " << token.token_name << endl;
            stream_process_input_token(token);
        } else {
            for (auto it = item.rhs.rbegin(); it != item.rhs.rend(); it++) {
                parse_stack.push(*it);
            }
        }
    }
}
