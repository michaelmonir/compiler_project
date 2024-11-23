#ifndef DFA_CONVERTOR
#define DFA_CONVERTOR

#define EPSLON 256
#define NFA_INPUT_SIZE 257
#define DFA_INPUT_SIZE 256
#define inf 100000000

#include <string>
#include <vector>

using namespace std;

typedef struct token_s
{
    int index;
    string token_name;

    bool operator<(const struct token_s& other) const {
        return index < other.index;
    }

    bool operator==(const struct token_s& other) const {
        return index == other.index && token_name == other.token_name;
    }
} token_t;

typedef struct dfa_node_s
{
    int dfa_node_index;
    struct dfa_node_s *neighbors[DFA_INPUT_SIZE];
    token_t token;
    static int dfa_nodes_counter;

    dfa_node_s() : dfa_node_index(dfa_nodes_counter++){ }
} dfa_node_t;

typedef struct nfa_node_s 
{
    int nfa_node_index;
    vector<vector<struct nfa_node_s *>> neighbors;
    token_t token;

    nfa_node_s() {}

    nfa_node_s(int index, vector<pair<int, struct nfa_node_s*>> edges, token_t token_input) 
        : nfa_node_index(index), neighbors(1), token(token_input)
    {
        for (auto [x, node] : edges) 
            neighbors[x].push_back(node);
    }
} nfa_node_t;

vector<dfa_node_t*> dfa_convertor_convert(nfa_node_t* start);

void print_dfa_nodes(vector<dfa_node_t*> start_nodes);

#endif // DFA_CONVERTOR