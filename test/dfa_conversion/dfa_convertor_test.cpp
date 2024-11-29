#include <gtest/gtest.h>
#include "dfa_convertor.h"

#include <iostream>

int set_dfa_initial_value = 1;

TEST(TRY_DFA_CONVERTOR, sheet_2_q_1_1) 
{
    DfaNode::dfa_nodes_counter = set_dfa_initial_value;

    cout << "q_1_1" << endl;
    NfaNode* a = new NfaNode(1);
    NfaNode* b = new NfaNode(2);
    NfaNode* c = new NfaNode(3);
    
    a->neighbors['0'].push_back(b);
    a->neighbors['1'].push_back(a);
    a->neighbors['1'].push_back(c);

    b->neighbors['0'].push_back(c);
    b->neighbors['1'].push_back(a);
    
    c->neighbors['0'].push_back(a);
    c->neighbors['1'].push_back(c);

    b->token.index = 2;
    b->token.token_name = "token b";

    vector<NfaNode*> start_nodes = {a, b};

    vector<DfaNode*> dfa_result = dfa_convertor_convert(start_nodes);

    print_dfa_nodes(dfa_result);
}

TEST(TRY_DFA_CONVERTOR, sheet_2_q_1_2) 
{
    DfaNode::dfa_nodes_counter = set_dfa_initial_value;

    cout << "q_1_2" << endl;
    NfaNode* a = new NfaNode(1);
    NfaNode* b = new NfaNode(2);
    NfaNode* c = new NfaNode(3);
    NfaNode* d = new NfaNode(4);
    
    a->neighbors['0'].push_back(b);
    a->neighbors['0'].push_back(c);
    a->neighbors['1'].push_back(d);
    a->neighbors['1'].push_back(c);

    b->neighbors['1'].push_back(d);

    c->neighbors['0'].push_back(a);
    c->neighbors['0'].push_back(b);
    c->neighbors['1'].push_back(c);

    d->neighbors['1'].push_back(a);
    d->neighbors['1'].push_back(c);

    d->token.index = 4;
    d->token.token_name = "token d";

    vector<NfaNode*> start_nodes = {a, b};

    vector<DfaNode*> dfa_result = dfa_convertor_convert(start_nodes);

    print_dfa_nodes(dfa_result);
}

TEST(TRY_DFA_CONVERTOR, sheet_2_q_1_3) 
{
    DfaNode::dfa_nodes_counter = set_dfa_initial_value;

    cout << "q_1_3" << endl;
    NfaNode* a = new NfaNode(1);
    NfaNode* b = new NfaNode(2);
    NfaNode* c = new NfaNode(3);
    NfaNode* d = new NfaNode(4);
    
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

    vector<NfaNode*> start_nodes = {a};

    vector<DfaNode*> dfa_result = dfa_convertor_convert(start_nodes);

    print_dfa_nodes(dfa_result);
}

TEST(TRY_DFA_CONVERTOR, sheet_2_q_3) 
{
    DfaNode::dfa_nodes_counter = set_dfa_initial_value;

    cout << "q_3" << endl;
    NfaNode* a = new NfaNode(1);
    NfaNode* b = new NfaNode(2);
    NfaNode* c = new NfaNode(3);
    NfaNode* d = new NfaNode(4);
    NfaNode* e = new NfaNode(5);
    NfaNode* f = new NfaNode(6);
    NfaNode* g = new NfaNode(7);
    
    a->neighbors[EPSLON].push_back(b);
    a->neighbors[EPSLON].push_back(c);
    a->neighbors[EPSLON].push_back(e);

    b->neighbors[EPSLON].push_back(c);
    b->neighbors['0'].push_back(d);
    b->neighbors['1'].push_back(d);

    c->neighbors[EPSLON].push_back(e);
    c->neighbors['0'].push_back(d);

    e->neighbors[EPSLON].push_back(b);
    e->neighbors['0'].push_back(e);

    d->neighbors[EPSLON].push_back(g);
    d->neighbors['0'].push_back(e);
    d->neighbors['1'].push_back(f);

    f->neighbors[EPSLON].push_back(g);
    f->neighbors['1'].push_back(e);

    g->token.index = 7;
    g->token.token_name = "token g";

    vector<NfaNode*> start_nodes = {a};

    vector<DfaNode*> dfa_result = dfa_convertor_convert(start_nodes);

    print_dfa_nodes(dfa_result);
}
