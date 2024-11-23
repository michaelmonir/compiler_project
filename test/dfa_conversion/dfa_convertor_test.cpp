#include <gtest/gtest.h>
#include "dfa_convertor.h"

TEST(TRY_DFA_CONVERTOR, two) {
    nfa_node_t* a = new nfa_node_t();
    nfa_node_t* b = new nfa_node_t();
    nfa_node_t* c = new nfa_node_t();
    nfa_node_t* d = new nfa_node_t();
    
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
}
