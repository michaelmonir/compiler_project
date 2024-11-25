#include <gtest/gtest.h>
#include "dfa_convertor.h"

#include <iostream>

/*
TEST(TRY_DFA_CONVERTOR, sheet_2_q_1) {
    cout << "a3" << endl;
    nfa_node_t* a = new nfa_node_t(1);
    nfa_node_t* b = new nfa_node_t(2);
    nfa_node_t* c = new nfa_node_t(3);
    
    a->neighbors['0'].push_back(b);
    a->neighbors['1'].push_back(a);
    a->neighbors['1'].push_back(c);

    b->neighbors['0'].push_back(c);
    b->neighbors['1'].push_back(a);
    
    c->neighbors['0'].push_back(a);
    c->neighbors['1'].push_back(c);

    b->token.index = 2;
    b->token.token_name = "token b";

    vector<dfa_node_t*> dfa_result = dfa_convertor_convert(a);

    print_dfa_nodes(dfa_result);
    int xx = 0;
    xx++;
}
*/

TEST(TRY_DFA_CONVERTOR, sheet_2_q_3) {
    cout << "a3" << endl;
    nfa_node_t* a = new nfa_node_t(1);
    nfa_node_t* b = new nfa_node_t(2);
    nfa_node_t* c = new nfa_node_t(3);
    nfa_node_t* d = new nfa_node_t(4);
    
    a->neighbors['0'].push_back(b);
    a->neighbors['0'].push_back(c);

    b->neighbors['1'].push_back(a);
    b->neighbors['1'].push_back(c);
    
    c->neighbors['0'].push_back(d);
    c->neighbors['1'].push_back(d);

    d->neighbors['0'].push_back(b);
    d->neighbors['1'].push_back(d);

    b->token.index = 2;
    b->token.token_name = "token b";

    c->token.index = 3;
    c->token.token_name = "token c";

    vector<dfa_node_t*> dfa_result = dfa_convertor_convert(a);

    print_dfa_nodes(dfa_result);
    int xx = 0;
    xx++;
}
