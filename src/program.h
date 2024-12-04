//
// Created by michael on 12/4/24.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include "dfa_conversion/dfa_convertor.h"
#include "nfa/nfa.h"
#include "RulesParser/RulesParser.h"
#include "minimal_dfa.h"

void lexical_analyze_file(string file_name)
{
    vector<Rule> rules = parseRules(file_name);
    NFA *nfa = new NFA(rules);

    NfaNode *nfa_start_node = nfa->get_root();
    vector nfa_start_nodes = {nfa_start_node};

    vector<DfaNode*> dfa_start_nodes = dfa_convertor_convert(nfa_start_nodes);
    vector<DfaNode*> minimal_dfa_start_nodes = minimize_dfa(dfa_start_nodes);

}

#endif //PROGRAM_H
