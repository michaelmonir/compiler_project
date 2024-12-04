#ifndef DFA_CONVERTOR
#define DFA_CONVERTOR

#include <vector>

#include "../RulesParser/structs.h"

using namespace std;

vector<DfaNode*> dfa_convertor_convert(vector<NfaNode*> start_nodes);

void print_dfa_nodes(vector<DfaNode*> start_nodes);

#endif // DFA_CONVERTOR
