//
// Created by mohamed arous on 11/29/2024.
//

#ifndef MINIMAL_DFA_H
#define MINIMAL_DFA_H

#include "../dfa_conversion/dfa_convertor.h"
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<dfa_node_t*> minimize_dfa(vector<dfa_node_t*> dfa_start_nodes);

#endif //MINIMAL_DFA_H
