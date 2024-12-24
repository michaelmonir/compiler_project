//
// Created by michael on 12/24/24.
//

#include "ParcerStream.h"
#include <stack>

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
ParseUnit start_symbol;

void initialize_stack() {
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
