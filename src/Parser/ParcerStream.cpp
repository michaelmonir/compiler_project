//
// Created by michael on 12/24/24.
//

#include "ParcerStream.h"
#include <stack>

void bbbb() {}

class Node {
public:
    ParseUnit unit;
    vector<Node*> children;

    Node(ParseUnit unit) : unit(unit) {}
};

map<ParseUnit, Token> terminal_map;
map<ParseUnit,map<Token, ParseTableItem>> parse_table;

stack<Node*> parse_stack;
Node *root;

void initialize_terminal_map_and_parse_table
    (map<ParseUnit, Token> _terminal_map,
     map<ParseUnit, map<Token, ParseTableItem>> _parse_table) {
    terminal_map = _terminal_map;
    parse_table = _parse_table;
}

void initialize_stack(ParseUnit start_symbol) {
    root = new Node(start_symbol);
    parse_stack.push(root);
}

void left_most_print_node(Node *node) {
    if (!node) return;

    if (node->unit.type == ParseUnitType::TERMINAL) {
        cout << terminal_map[node->unit].token_name << " ";
    } else {
        for (Node* child : node->children) {
            left_most_print_node(child);
        }
    }
}

void left_most_print_root() {
    left_most_print_node(root);
}

void stream_process_input_token(Token token) {
    if (parse_stack.empty()) {
        cout << "EMPTY STACK: Error at token " << token.token_name << endl;
        return;
    }
    Node* topNode = parse_stack.top();
    ParseUnit top = topNode->unit;

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
            parse_stack.pop();
            for (auto it = item.rhs.rbegin(); it != item.rhs.rend(); it++) {
                Node *newNode = new Node(*it);
                topNode->children.push_back(newNode);
                parse_stack.push(newNode);
            }
        }
    }
}


// void initialize_terminal_map_and_parse_table
//     (map<ParseUnit, Token> _terminal_map,
//      map<ParseUnit, map<Token, ParseTableItem>> _parse_table);
void initialize_terminal_map_and_parse_table_adapt
    (set<string> terminal_set,
    std::map<std::string, std::map<std::string, std::pair<bool, std::vector<ParseUnit>>>> old_parse_unit) {

    map<ParseUnit, Token> _terminal_map;
    map<ParseUnit, map<Token, ParseTableItem>> _parse_table;

    for (auto it = terminal_set.begin(); it != terminal_set.end(); it++) {
        ParseUnit b(*it, ParseUnitType::TERMINAL);
        Token t(0, b.lhs);
        _terminal_map[b] = t;
    }
    for (auto it = old_parse_unit.begin(); it != old_parse_unit.end(); it++) {
        ParseUnit b(it->first, ParseUnitType::NON_TERMINAL);
        for (auto it2 = it->second.rbegin(); it2 != it->second.rend(); it2) {

            Token t(0, it->first);
            _parse_table[b][t] = ParseTableItem(it2->second.first, it2->second.second);
        }
    }

    initialize_terminal_map_and_parse_table(terminal_map, _parse_table);
}
