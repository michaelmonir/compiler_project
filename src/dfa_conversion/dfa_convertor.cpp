#include "dfa_convertor.h"

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <iostream>

using namespace std;

int dfa_node_t::dfa_nodes_counter = 0;

const token_t NO_TOKEN = {
    .index = inf,
    .token_name = ""
};

token_t get_token_nfa_nodes(vector<nfa_node_t*> input);
bool check_start_dfa_node(vector<int> indeces, int start_index);
void add_epslon_neighbors_to_set(nfa_node_t* start_node, map<int, nfa_node_t*> &nodes_map);
vector<int> get_indeces_from_nodes(vector<nfa_node_t*> nodes);
vector<nfa_node_t*> get_neighbors_for_input(vector<nfa_node_t*> current_nodes, int input);

vector<dfa_node_t*> dfa_convertor_convert(nfa_node_t* start) 
{
    vector<dfa_node_t*> start_dfa_nodes;

    int start_index = start->nfa_node_index;
    map<vector<int>, dfa_node_t*> mapp;

    queue<pair<vector<nfa_node_t*>, dfa_node_t*>> q;

    {
        vector<nfa_node_t*> start_vector = {start};
        dfa_node_t* start_dfa_node_1 = new dfa_node_t();
        q.push({start_vector, start_dfa_node_1});
    }

    while (q.size())
    {
        // get nfa nodes and their equivilant dfa node from queue
        vector<nfa_node_t*> start_nodes = q.front().first;
        dfa_node_t *dfa_node = q.front().second;
        q.pop();

        // set the token of the dfa node
        token_t token = get_token_nfa_nodes(start_nodes); 
        dfa_node->token = token;

        // checking if node is a dfa start node
        vector<int> incoming_indeces = get_indeces_from_nodes(start_nodes);
        if (check_start_dfa_node(incoming_indeces, start_index)) {
            start_dfa_nodes.push_back(dfa_node);
        }

        // for each input determine the next dfa node
        // if dfa node for same nfa nodes create before, reuse it using the map
        for (int input = 0; input < DFA_INPUT_SIZE; input++) 
        {
            vector<nfa_node_t*> neighbors = get_neighbors_for_input(start_nodes, input);
            vector<int> neighbor_indeces = get_indeces_from_nodes(neighbors);
            
            dfa_node_t* next_dfa_node;

            if (mapp.count(neighbor_indeces))
            {
                next_dfa_node = mapp[neighbor_indeces];
            }
            else
            {
                next_dfa_node = new dfa_node_t();
                mapp[neighbor_indeces] = next_dfa_node;
            }
        }
    }

    return start_dfa_nodes;
}

token_t get_token_nfa_nodes(vector<nfa_node_t*> input)
{
    token_t res = NO_TOKEN;

    for (nfa_node_t* node : input)
    {
        if (node->token.index < res.index)
            res = node->token;
    }

    return res;
}

vector<nfa_node_t*> get_neighbors_for_input(vector<nfa_node_t*> current_nodes, int input)
{
    map<int, nfa_node_t*> nodes_mapp;

    for (nfa_node_t* cur_node : current_nodes) {
        for (nfa_node_t* node : cur_node->neighbors[input]) {
            add_epslon_neighbors_to_set(node, nodes_mapp);
        } 
    }

    vector<nfa_node_t*> result;

    for (auto [id, node] : nodes_mapp) {
        result.push_back(node);
    }
    return result;
}

void add_epslon_neighbors_to_set(nfa_node_t* start_node, map<int, nfa_node_t*> &nodes_map)
{
    queue<nfa_node_t*> q;
    q.push(start_node);

    while (q.size())
    {
        nfa_node_t* node = q.front(); q.pop();
        if (nodes_map.count(node->nfa_node_index)) continue;

        nodes_map[node->nfa_node_index] = node;

        for (nfa_node_t *child : node->neighbors[EPSLON])
            q.push(child);
    }
}

bool check_start_dfa_node(vector<int> indeces, int start_index)
{
    for (int index : indeces) 
        if (start_index == index) return true;
    return false;
}

vector<int> get_indeces_from_nodes(vector<nfa_node_t*> nodes) 
{
    int n = nodes.size();
    vector<int> indeces(n);

    for (int i = 0; i < n; i++) 
        indeces[i] = nodes[i]->nfa_node_index;
    return indeces;
}

void print_dfa_nodes(vector<dfa_node_t*> start_nodes)
{
    queue<dfa_node_t*> q;
    set<int> visited;

    for (dfa_node_t* node : start_nodes) q.push(node);

    while (q.size())
    {
        dfa_node_t* node = q.front(); q.pop();
        if (visited.count(node->dfa_node_index)) continue;

        for (int input = 0; input < DFA_INPUT_SIZE; input++)
        {
            if (NULL == node->neighbors[input]) continue;

            cout << "from: " << node->dfa_node_index << " ";
            cout << "to: " << node->neighbors[input]->dfa_node_index << " ";
            cout << "input: " << input << endl;
        }
    }
}