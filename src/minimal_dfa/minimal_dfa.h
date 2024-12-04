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

vector<DfaNode*> minimize_dfa(vector<DfaNode*> dfa_start_nodes);

#endif //MINIMAL_DFA_H
