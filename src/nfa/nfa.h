#ifndef NFA_H
#define NFA_H

#include <vector>
#include <map>
#include "../RulesParser/string_to_relation_converter.h"

using namespace std;

// NFA Node structure
struct nfa_node
{
    vector<vector<nfa_node *>> neighbors;
    bool is_final;

    nfa_node();
};

// Structure to hold start and end nodes
struct start_and_end_nodes
{
    nfa_node *start_node;
    nfa_node *end_node;
};

// NFA class
class NFA
{
public:
    nfa_node *root{};
    vector<Rule> rules;
    map<int, start_and_end_nodes> symbol_id_to_start_and_end_node;
    map<int, int> symbol_id_to_rule_index;

    explicit NFA(const vector<Rule> &rules);

    static start_and_end_nodes apply_char_rule(const CharRelation& relation);
    static start_and_end_nodes apply_or_rule(const vector<nfa_node *>& start_nodes, const vector<nfa_node *>& end_nodes);
    static start_and_end_nodes apply_and_rule(const vector<nfa_node *> &start_nodes, const vector<nfa_node *> &end_nodes);
    static start_and_end_nodes apply_closure_rule(nfa_node *start_node, nfa_node *end_node);
    start_and_end_nodes apply_symbol_rule(const Symbol *symbol);
    start_and_end_nodes apply_rule(Relation *relation);
    void construct_nfa();

    nfa_node *get_root() const;
    nfa_node *deep_copy_tree(nfa_node *original_node, nfa_node *&new_end_node);
};

#endif // NFA_H
