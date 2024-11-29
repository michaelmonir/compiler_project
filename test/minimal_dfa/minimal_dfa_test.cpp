//
// Created by mohamed arous on 11/29/2024.
//

#include <gtest/gtest.h>
#include "minimal_dfa.h"

#include <iostream>
int set_dfa_initial_value1 = 1;


TEST(MINIMAL_DFA, no_minimization_needed)
{
    dfa_node_t::dfa_nodes_counter = set_dfa_initial_value1;

    cout << "no_minimization_needed" << endl;
    dfa_node_t* a = new dfa_node_t();
    dfa_node_t* b = new dfa_node_t();

    a->neighbors['0'] = b;
    a->neighbors['1'] = a;

    b->neighbors['0'] = b;
    b->neighbors['1'] = a;

    b->token.index = 2;
    b->token.token_name = "accepting state";

    vector<dfa_node_t*> start_nodes = {a};

    vector<dfa_node_t*> minimal_dfa = minimize_dfa(start_nodes);

    print_dfa_nodes(minimal_dfa);

    ASSERT_EQ(minimal_dfa.size(), 2); // Expect no reduction
}
