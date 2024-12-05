#ifndef NFA_H
#define NFA_H

#include <vector>
#include <map>
#include "../RulesParser/string_to_relation_converter.h"

using namespace std;

struct start_and_end_nodes
{
    NfaNode *start_node;
    NfaNode *end_node;
};

// NFA class
class NFA
{
public:
    NfaNode *root{};
    vector<Rule> rules;
    map<string, start_and_end_nodes> symbol_id_to_start_and_end_node;
    map<string, int> symbol_id_to_rule_index;

    explicit NFA(const vector<Rule> &rules);

    static start_and_end_nodes apply_char_rule(const CharRelation& relation);
    static start_and_end_nodes apply_or_rule(const vector<NfaNode *>& start_nodes, const vector<NfaNode *>& end_nodes);
    static start_and_end_nodes apply_and_rule(const vector<NfaNode *> &start_nodes, const vector<NfaNode *> &end_nodes);
    static start_and_end_nodes apply_string_rule(const StringRelation &relation);
    static start_and_end_nodes apply_range_rule(const RangeRelation &relation);
    static start_and_end_nodes apply_closure_rule(NfaNode *start_node, NfaNode *end_node,bool is_postive);
    start_and_end_nodes apply_symbol_rule(const Symbol *symbol);
    start_and_end_nodes apply_rule(Relation *relation);
    void construct_nfa();

    NfaNode *get_root() const;
};

#endif // NFA_H
