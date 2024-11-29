#ifndef DFA_CONVERTOR
#define DFA_CONVERTOR

#define EPSLON 256
#define NFA_INPUT_SIZE 257
#define DFA_INPUT_SIZE 256
#define inf 100000000

#include <string>
#include <vector>
#include <map>

using namespace std;

class Token {
public:
    int index;
    string token_name;

    static Token NO_TOKEN;

    bool operator<(const Token& other) const {
        return index < other.index;
    }

    bool operator==(const Token& other) const {
        return index == other.index && token_name == other.token_name;
    }
};

class DfaNode {
public:
    static int dfa_nodes_counter;

    int dfa_node_index;
    map<int, DfaNode*> neighbors;
    Token token;

    DfaNode() : dfa_node_index(dfa_nodes_counter++) {}
};

class NfaNode {
public:
    int nfa_node_index;
    vector<vector<NfaNode*>> neighbors;
    Token token;

    NfaNode(int index)
        : nfa_node_index(index), neighbors(NFA_INPUT_SIZE), token(Token::NO_TOKEN) {}

    NfaNode(int index, vector<pair<int, NfaNode*>> edges, Token token_input)
        : nfa_node_index(index), neighbors(NFA_INPUT_SIZE), token(token_input)
    {
        for (auto [x, node] : edges)
            neighbors[x].push_back(node);
    }
};

vector<DfaNode*> dfa_convertor_convert(vector<NfaNode*> start_nodes);

void print_dfa_nodes(vector<DfaNode*> start_nodes);

#endif // DFA_CONVERTOR
