//
// Created by michael on 12/4/24.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include "dfa_conversion/dfa_convertor.h"
#include "nfa/nfa.h"
#include "RulesParser/RulesParser.h"

void lexical_analyze_file(string file_name)
{
    vector<Rule> rules = parseRules(file_name);
    NFA *nfa = new NFA(rules);

    nfa->get_root();
}

#endif //PROGRAM_H
