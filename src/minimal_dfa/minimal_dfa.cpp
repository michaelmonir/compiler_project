//
// Created by mohamed arous on 11/29/2024.
//

#include "minimal_dfa.h"

#include <algorithm>

vector<DfaNode *> get_new_start_nodes(
    vector<DfaNode *> nodes,
    map<int, int> state_group,
    vector<DfaNode *> dfa_start_nodes);
map<int, DfaNode *> collect_nodes(vector<DfaNode *> dfa_start_nodes);

vector<vector<bool>> get_propagated_distinguishable_table(map<int, DfaNode *> all_nodes, map<int, int> index_map);

vector<DfaNode*> minimize_dfa(vector<DfaNode*> dfa_start_nodes) {
    map<int, DfaNode*> all_nodes = collect_nodes(dfa_start_nodes);

    // Map indices to compact range
    map<int, int> index_map;
    int compact_index = 0;
    for (const auto& [old_index, node] : all_nodes) {
        index_map[old_index] = compact_index++;
    }

    vector<vector<bool>> distinguishable = get_propagated_distinguishable_table(all_nodes, index_map);

    // Group indistinguishable states
    map<int, int> state_group;
    int group_counter = 0;

    for (auto& [old_index, node] : all_nodes) {
        if (state_group.count(old_index)) continue;

        state_group[old_index] = group_counter;
        for (auto& [other_index, other_node] : all_nodes) {
            if (old_index != other_index &&
                !distinguishable[min(index_map[old_index], index_map[other_index])][max(index_map[old_index], index_map[other_index])]) {
                state_group[other_index] = group_counter;
                }
        }
        group_counter++;
    }

    // Create minimized nodes
    vector<DfaNode*> minimized_nodes(group_counter);
    for (int i = 0; i < group_counter; i++) {
        minimized_nodes[i] = new DfaNode();
    }

    // Set transitions and tokens for minimized DFA
    for (auto& [old_index, group] : state_group) {
        DfaNode* old_node = all_nodes[old_index];
        DfaNode* new_node = minimized_nodes[group];
        new_node->token = old_node->token;

        for (auto& [input, neighbor] : old_node->neighbors) {
            if (neighbor) {
                int new_neighbor_group = state_group[neighbor->dfa_node_index];
                new_node->neighbors[input] = minimized_nodes[new_neighbor_group];
            }
        }
    }

    return get_new_start_nodes(minimized_nodes, state_group, dfa_start_nodes);
}


vector<DfaNode*> get_new_start_nodes(vector<DfaNode*> minimized_nodes,
                                        map<int,int> state_group,
                                        vector<DfaNode*> dfa_start_nodes) {
    vector<DfaNode*> new_start_nodes;
    for (DfaNode* node : dfa_start_nodes) {
        int group = state_group[node->dfa_node_index];
        if (find(new_start_nodes.begin(),
             new_start_nodes.end(),
                 minimized_nodes[group]) == new_start_nodes.end()) {
            new_start_nodes.push_back(minimized_nodes[group]);
        }
    }
    return  new_start_nodes;
}

map<int, DfaNode*> collect_nodes(vector<DfaNode*> dfa_start_nodes) {
    map<int, DfaNode*> all_nodes;
    queue<DfaNode*> q;
    set<int> visited;

    for (DfaNode* node : dfa_start_nodes)
        q.push(node);

    while (!q.empty()) {
        DfaNode* node = q.front(); q.pop();
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


vector<vector<bool>> get_propagated_distinguishable_table(
    map<int, DfaNode*> all_nodes,
    map<int, int> index_map)
{
    int node_count = all_nodes.size();
    vector<vector<bool>> distinguishable(node_count, vector<bool>(node_count, false));

    // Mark distinguishable states based on tokens
    for (auto &[i, node1] : all_nodes) {
        for (auto &[j, node2] : all_nodes) {
            int idx_i = index_map[i];
            int idx_j = index_map[j];
            if (idx_i < idx_j && !(node1->token == node2->token)) {
                distinguishable[idx_i][idx_j] = true;
            }
        }
    }

    // Propagate distinguishability
    bool changed;
    do {
        changed = false;
        for (auto &[i, node1] : all_nodes) {
            for (auto &[j, node2] : all_nodes) {
                int idx_i = index_map[i];
                int idx_j = index_map[j];
                if (idx_i >= idx_j || distinguishable[idx_i][idx_j]) continue;

                for (int input = 0; input < DFA_INPUT_SIZE; input++) {
                    if (!node1->neighbors.count(input) || !node2->neighbors.count(input)) {
                        continue;
                    }
                    DfaNode* neighbor1 = node1->neighbors[input];
                    DfaNode* neighbor2 = node2->neighbors[input];

                    int neighbor_idx1 = index_map[neighbor1->dfa_node_index];
                    int neighbor_idx2 = index_map[neighbor2->dfa_node_index];
                    if (distinguishable[min(neighbor_idx1, neighbor_idx2)]
                                        [max(neighbor_idx1, neighbor_idx2)]) {
                        distinguishable[idx_i][idx_j] = true;
                        changed = true;
                        break;
                                        }
                }
            }
        }

    } while (changed);

    return distinguishable;
}

