//
// Created by mohamed arous on 11/29/2024.
//

#include <gtest/gtest.h>
#include "minimal_dfa.h"

#include <iostream>
int set_dfa_initial_value1 = 1;


// TEST(MINIMAL_DFA, no_minimization_needed)
// {
//     DfaNode::dfa_nodes_counter = set_dfa_initial_value1;
//
//     cout << "no_minimization_needed" << endl;
//     DfaNode* a = new DfaNode();
//     DfaNode* b = new DfaNode();
//
//     a->neighbors['0'] = b;
//     a->neighbors['1'] = a;
//
//     b->neighbors['0'] = b;
//     b->neighbors['1'] = a;
//
//
//     b->token.index = 2;
//     b->token.token_name = "accepting state";
//
//     vector<DfaNode*> start_nodes = {a};
//
//     vector<DfaNode*> minimal_dfa = minimize_dfa(start_nodes);
//
//     print_dfa_nodes(minimal_dfa);
// }

TEST(MINIMAL_DFA, equivalent_states)
{
    DfaNode::dfa_nodes_counter = set_dfa_initial_value1;

    cout << "equivalent_states" << endl;
    DfaNode* a = new DfaNode();
    DfaNode* b = new DfaNode();
    DfaNode* c = new DfaNode();

    a->neighbors['0'] = b;
    a->neighbors['1'] = c;

    b->neighbors['0'] = b;
    b->neighbors['1'] = c;

    c->neighbors['0'] = b;
    c->neighbors['1'] = c;

    b->token.index = 2;
    b->token.token_name = "accepting state";

    c->token.index = 2; // Same token as state `b`
    c->token.token_name = "accepting state";
    vector<DfaNode*> start_nodes = {a};

    vector<DfaNode*> minimal_dfa = minimize_dfa(start_nodes);

    print_dfa_nodes(minimal_dfa);
}

// TEST(MINIMAL_DFA, distinguishable_states)
// {
//     DfaNode::dfa_nodes_counter = set_dfa_initial_value1+6;
//
//     cout << "distinguishable_states" << endl;
//     DfaNode* a = new DfaNode();
//     DfaNode* b = new DfaNode();
//     DfaNode* c = new DfaNode();
//
//     a->neighbors['0'] = b;
//     a->neighbors['1'] = c;
//
//     b->neighbors['0'] = b;
//     b->neighbors['1'] = a;
//
//     c->neighbors['0'] = c;
//     c->neighbors['1'] = a;
//
//     b->token.index = 2;
//     b->token.token_name = "token b";
//
//     c->token.index = 3;
//     c->token.token_name = "token c"; // Distinguishable from `b`
//
//     vector<DfaNode*> start_nodes = {a};
//
//     vector<DfaNode*> minimal_dfa = minimize_dfa(start_nodes);
//
//     print_dfa_nodes(minimal_dfa);
//
//     //ASSERT_EQ(minimal_dfa.size(), 3); // No states should merge
// }

