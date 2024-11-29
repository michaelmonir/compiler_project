//
// Created by mohamed arous on 11/29/2024.
//

#include "minimal_dfa.h"

#include <algorithm>

vector<dfa_node_t *> get_new_start_nodes(
    vector<dfa_node_t *> nodes,
    map<int, int> state_group,
    vector<dfa_node_t *> dfa_start_nodes);
map<int, dfa_node_t *> collect_nodes(vector<dfa_node_t *> dfa_start_nodes);

vector<vector<bool>> get_propagated_distinguishable_table(map<int, dfa_node_t *> all_nodes);

vector<dfa_node_t*> minimize_dfa(vector<dfa_node_t*> dfa_start_nodes)
{
    map<int, dfa_node_t*> all_nodes = collect_nodes(dfa_start_nodes);
    vector<vector<bool>> distinguishable = get_propagated_distinguishable_table(all_nodes);

    // Group indistinguishable states
    map<int, int> state_group;
    int group_counter = 0;

    for (auto &[i, node1] : all_nodes) {
        if (state_group.count(i)) continue;

        state_group[i] = group_counter;
        for (auto &[j, node2] : all_nodes) {
            if (i != j && !distinguishable[min(i, j)][max(i, j)]) {
                state_group[j] = group_counter;
            }
        }
        group_counter++;
    }

    // Create the new minimized DFA nodes
    vector<dfa_node_t*> minimized_nodes(group_counter);
    for (int i = 0; i < group_counter; i++) {
        minimized_nodes[i] = new dfa_node_t();
    }

    // Set transitions and tokens for the minimized DFA
    for (auto &[old_index, group] : state_group) {
        dfa_node_t* old_node = all_nodes[old_index];
        dfa_node_t* new_node = minimized_nodes[group];
        new_node->token = old_node->token;

        for (auto &[input, neighbor] : old_node->neighbors) {
            if (neighbor) {
                int new_neighbor_group = state_group[neighbor->dfa_node_index];
                new_node->neighbors[input] = minimized_nodes[new_neighbor_group];
            }
        }
    }

    return get_new_start_nodes(minimized_nodes, state_group, dfa_start_nodes);
}

vector<dfa_node_t*> get_new_start_nodes(vector<dfa_node_t*> minimized_nodes,
                                        map<int,int> state_group,
                                        vector<dfa_node_t*> dfa_start_nodes) {
    vector<dfa_node_t*> new_start_nodes;
    for (dfa_node_t* node : dfa_start_nodes) {
        int group = state_group[node->dfa_node_index];
        if (find(new_start_nodes.begin(),
             new_start_nodes.end(),
                 minimized_nodes[group]) == new_start_nodes.end()) {
            new_start_nodes.push_back(minimized_nodes[group]);
        }
    }
    return  new_start_nodes;
}

map<int, dfa_node_t*> collect_nodes(vector<dfa_node_t*> dfa_start_nodes) {
    map<int, dfa_node_t*> all_nodes;
    queue<dfa_node_t*> q;
    set<int> visited;

    for (dfa_node_t* node : dfa_start_nodes)
        q.push(node);

    while (!q.empty()) {
        dfa_node_t* node = q.front(); q.pop();
        if (visited.count(node->dfa_node_index)) continue;

        visited.insert(node->dfa_node_index);
        all_nodes[node->dfa_node_index] = node;

        for (auto [input, neighbor] : node->neighbors) {
            if (neighbor && !visited.count(neighbor->dfa_node_index)) {
                q.push(neighbor);
            }
        }
    }
    return all_nodes;
}


vector<vector<bool>> get_propagated_distinguishable_table(map<int, dfa_node_t*> all_nodes) {
    int node_count = all_nodes.size();
    vector<vector<bool>> distinguishable(node_count, vector<bool>(node_count, false));

    // Mark distinguishable states based on tokens
    for (auto &[i, node1] : all_nodes) {
        for (auto &[j, node2] : all_nodes) {
            if (i < j && !(node1->token == node2->token)) {
                distinguishable[i][j] = true;
            }
        }
    }

    // Propagate distinguishability
    bool changed;
    do {
        changed = false;
        for (auto &[i, node1] : all_nodes) {
            for (auto &[j, node2] : all_nodes) {
                if (i >= j || distinguishable[i][j]) continue;

                for (int input = 0; input < DFA_INPUT_SIZE; input++) {
                    dfa_node_t* neighbor1 = node1->neighbors[input];
                    dfa_node_t* neighbor2 = node2->neighbors[input];

                    if (neighbor1
                        && neighbor2
                        && distinguishable[neighbor1->dfa_node_index][neighbor2->dfa_node_index]) {
                        distinguishable[i][j] = true;
                        changed = true;
                        break;
                        }
                }
            }
        }
    } while (changed);
    return distinguishable;
}
