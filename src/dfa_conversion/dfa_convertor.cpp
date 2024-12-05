#include "dfa_convertor.h"

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <iostream>

using namespace std;

int DfaNode::dfa_nodes_counter = 1;

Token get_token_nfa_nodes(vector<NfaNode*> input);
bool check_start_dfa_node(vector<int> indeces, set<int> start_indeces);
void add_node_and_epslon_neighbors_to_map(NfaNode* start_node, map<int, NfaNode*> &nodes_map);
vector<int> get_indeces_from_nodes(vector<NfaNode*> nodes);
vector<NfaNode*> get_neighbors_for_input(vector<NfaNode*> current_nodes, int input);
vector<NfaNode*> get_neighboring_epslons(vector<NfaNode*> start_nodes);

vector<DfaNode*> dfa_convertor_convert(vector<NfaNode*> start_nodes) 
{
    vector<DfaNode*> start_dfa_nodes;

    set<int> start_indeces;

    for (NfaNode* node : start_nodes)
        start_indeces.emplace(node->nfa_node_index);

    map<vector<int>, DfaNode*> mapp;
    queue<pair<vector<NfaNode*>, DfaNode*>> q;

    {
        start_nodes = get_neighboring_epslons(start_nodes);

        DfaNode* start_dfa_node = new DfaNode();
        q.push({start_nodes, start_dfa_node});
        vector<int> start_indeces = get_indeces_from_nodes(start_nodes);
        mapp[start_indeces] = start_dfa_node;
    }

    while (q.size())
    {
        // get nfa nodes and their equivilant dfa node from queue
        vector<NfaNode*> start_nodes = q.front().first;
        DfaNode *dfa_node = q.front().second;
        q.pop();

        // set the token of the dfa node
        Token token = get_token_nfa_nodes(start_nodes); 
        dfa_node->token = token;

        // checking if node is a dfa start node
        vector<int> incoming_indeces = get_indeces_from_nodes(start_nodes);
        if (check_start_dfa_node(incoming_indeces, start_indeces)) {
            start_dfa_nodes.push_back(dfa_node);
        }

//        cout << "nfa nodes of dfa node #" << dfa_node->dfa_node_index << endl;
//        for (int index : incoming_indeces)
//          cout << index << " ";
//        cout << endl;

        // for each input determine the next dfa node
        // if dfa node for same nfa nodes create before, reuse it using the map
        for (int input = 0; input < DFA_INPUT_SIZE; input++) 
        {
            vector<NfaNode*> neighbors = get_neighbors_for_input(start_nodes, input);
            vector<int> neighbor_indeces = get_indeces_from_nodes(neighbors);

            if (0 == neighbors.size()) continue;
            
            DfaNode* next_dfa_node;

            if (mapp.count(neighbor_indeces))
            {
                next_dfa_node = mapp[neighbor_indeces];
            }
            else
            {
                next_dfa_node = new DfaNode();
                mapp[neighbor_indeces] = next_dfa_node;

                q.push({neighbors, next_dfa_node});
            }
            dfa_node->neighbors[input] = next_dfa_node;
        }
    }

    return start_dfa_nodes;
}

Token get_token_nfa_nodes(vector<NfaNode*> input)
{
    Token res = Token::NO_TOKEN;

    for (NfaNode* node : input)
    {
        if (node->token.index < res.index)
            res = node->token;
    }

    return res;
}

vector<NfaNode*> get_neighbors_for_input(vector<NfaNode*> current_nodes, int input)
{
    map<int, NfaNode*> nodes_mapp;

    for (NfaNode* cur_node : current_nodes) {
        for (NfaNode* node : cur_node->neighbors[input]) {
            add_node_and_epslon_neighbors_to_map(node, nodes_mapp);
        } 
    }

    vector<NfaNode*> result;

    for (auto [id, node] : nodes_mapp) {
        result.push_back(node);
    }
    return result;
}

vector<NfaNode*> get_neighboring_epslons(vector<NfaNode*> start_nodes)
{
    map<int, NfaNode*> mapp;
    for (NfaNode* node : start_nodes) 
        add_node_and_epslon_neighbors_to_map(node, mapp);

    vector<NfaNode*> result;

    for (auto [id, node] : mapp) {
        result.push_back(node);
    }

    return result;   
}

void add_node_and_epslon_neighbors_to_map(NfaNode* start_node, map<int, NfaNode*> &nodes_map)
{
    queue<NfaNode*> q;
    q.push(start_node);

    while (q.size())
    {
        NfaNode* node = q.front(); q.pop();
        if (nodes_map.count(node->nfa_node_index)) continue;

        nodes_map[node->nfa_node_index] = node;

        for (NfaNode *child : node->neighbors[EPSLON])
            q.push(child);
    }
}

bool check_start_dfa_node(vector<int> indeces, set<int> start_indeces)
{
    // if (indeces.size() != 1) return false;
    for (int index : indeces) 
        if (start_indeces.count(index)) return true;
    return false;
}

vector<int> get_indeces_from_nodes(vector<NfaNode*> nodes) 
{
    int n = nodes.size();
    vector<int> indeces(n);

    for (int i = 0; i < n; i++) 
        indeces[i] = nodes[i]->nfa_node_index;
    return indeces;
}

void print_dfa_nodes(vector<DfaNode*> start_nodes)
{
    queue<DfaNode*> q;
    set<int> visited;

    for (DfaNode* node : start_nodes) q.push(node);

    vector<pair<int, int>> nodes_with_tokens;

    while (q.size())
    {
        DfaNode* node = q.front(); q.pop();
        if (visited.count(node->dfa_node_index)) continue;
        visited.emplace(node->dfa_node_index);

        if (inf != node->token.index) {
            nodes_with_tokens.push_back({node->dfa_node_index, node->token.index});
        }

        for (int input = 0; input < DFA_INPUT_SIZE; input++)
        {
            if (NULL == node->neighbors[input]) continue;
            char c = input;

//            cout << "from: " << node->dfa_node_index << " ";
//            cout << "to: " << node->neighbors[input]->dfa_node_index << " ";
//            cout << "input: " << c << endl;

            q.push(node->neighbors[input]);
        }
    }

//    for (auto [x, y] : nodes_with_tokens)
//        cout << "node " << x << " token index: " << y << endl;
}