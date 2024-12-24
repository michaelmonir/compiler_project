#include <vector>
#include <map>
#include <iostream>
#include "nfa.h"

int NfaNode::nfa_nodes_counter = 1;

NFA::NFA(const std::vector<Rule> &rules) : rules(rules)
{
    construct_nfa();
}

start_and_end_nodes NFA::apply_char_rule(const CharRelation& relation)
{
    auto *starting_node = new NfaNode();
    auto *ending_node = new NfaNode();
    ending_node->is_final = true;
    starting_node->neighbors[relation.c].push_back(ending_node);
    const start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_string_rule(const StringRelation &relation)
{
    auto *starting_node = new NfaNode();
    auto *ending_node = new NfaNode();
    ending_node->is_final = true;

    NfaNode *current_node = starting_node;
    for (char c : relation.s)
    {
        auto *new_node = new NfaNode();
        current_node->neighbors[c].push_back(new_node);
        current_node = new_node;
    }

    current_node->neighbors[EPSLON].push_back(ending_node); // epsilon transition

    const start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_or_rule(const std::vector<NfaNode *>& start_nodes, const std::vector<NfaNode *>& end_nodes)
{
    auto *starting_node = new NfaNode();
    auto *ending_node = new NfaNode();
    ending_node->is_final = true;

    // Connect all start nodes to a new starting node via epsilon transitions
    for (auto *start_node : start_nodes)
    {
        starting_node->neighbors[EPSLON].push_back(start_node); // epsilon transition
    }

    // Connect all end nodes to a new ending node via epsilon transitions
    for (auto *end_node : end_nodes)
    {
        end_node->is_final = false;
        end_node->neighbors[EPSLON].push_back(ending_node); // epsilon transition
    }

    start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_and_rule(const std::vector<NfaNode *> &start_nodes, const std::vector<NfaNode *> &end_nodes)
{
    auto *starting_node = new NfaNode();
    auto *ending_node = new NfaNode();
    ending_node->is_final = true;

    // Link the first start node to the first start node via epsilon transition
    starting_node->neighbors[EPSLON].push_back(start_nodes[0]);

    // Link intermediate end nodes to the next start nodes via epsilon transitions
    for (size_t i = 0; i < start_nodes.size() - 1; i++)
    {
        end_nodes[i]->is_final = false;
        end_nodes[i]->neighbors[EPSLON].push_back(start_nodes[i + 1]); // epsilon transition
    }

    // Link the last end node to the final node via epsilon transition
    end_nodes[start_nodes.size() - 1]->is_final = false;
    end_nodes[start_nodes.size() - 1]->neighbors[EPSLON].push_back(ending_node);

    start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_closure_rule(NfaNode *start_node, NfaNode *end_node, bool is_postive)
{
    auto *starting_node = new NfaNode();
    auto *ending_node = new NfaNode();
    ending_node->is_final = true;
    end_node->is_final = false;

    end_node->neighbors[EPSLON].push_back(ending_node);     // epsilon transition
    starting_node->neighbors[EPSLON].push_back(start_node); // epsilon transition
    ending_node->neighbors[EPSLON].push_back(starting_node);      // epsilon transition
    if (!is_postive) {
        starting_node->neighbors[EPSLON].push_back(ending_node);      // epsilon transition
    }
    const start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_range_rule(const RangeRelation &relation)
{
    auto *starting_node = new NfaNode();
    auto *ending_node = new NfaNode();
    ending_node->is_final = true;

    for (char c = relation.c1; c <= relation.c2; c++)
    {
        auto *node1 = new NfaNode();
        auto *node2 = new NfaNode();
        node1->neighbors[c].push_back(node2);
        starting_node->neighbors[EPSLON].push_back(node1);
        node2->neighbors[EPSLON].push_back(ending_node);
    }

    const start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_symbol_rule(const Symbol *symbol)
{
        Relation* x = rules[symbol_id_to_rule_index[symbol->symbol_name]].relation;
        symbol_id_to_start_and_end_node[symbol->symbol_name] = apply_rule(x);
        return apply_rule(x);
}

start_and_end_nodes NFA::apply_rule(Relation *relation)
{
    if (dynamic_cast<CharRelation *>(relation))
    {
        // Apply char rule
        return apply_char_rule(*dynamic_cast<CharRelation *>(relation));
    }
    else if (dynamic_cast<symbolRelation *>(relation))
    {
        // Apply symbol rule
        return apply_symbol_rule((*dynamic_cast<symbolRelation *>(relation)).symbol);
    }else if(dynamic_cast<StringRelation *>(relation))
    {
        // Apply string rule
        return apply_string_rule(*dynamic_cast<StringRelation *>(relation));
    }
    else if (dynamic_cast<RangeRelation *>(relation))
    {
        // Apply range rule
        return apply_range_rule(*dynamic_cast<RangeRelation *>(relation));
    }
    else if (dynamic_cast<ClosureRelation *>(relation))
    {
        // Apply closure rule (Kleene star)
        const auto *closure_relation = dynamic_cast<ClosureRelation *>(relation);
        auto [start_node, end_node] = apply_rule(closure_relation->relation);
        return apply_closure_rule(start_node, end_node, closure_relation->is_positive);
    }
    else
    {
        // Handle OrRelation and AndRelation
        std::vector<NfaNode *> start_nodes, end_nodes;
        if (dynamic_cast<OrRelation *>(relation))
        {
            // Apply OR rule
            const auto *or_relation = dynamic_cast<OrRelation *>(relation);

            start_and_end_nodes sub_nodes = apply_rule(or_relation->r1);
            start_nodes.push_back(sub_nodes.start_node);
            end_nodes.push_back(sub_nodes.end_node);
            sub_nodes = apply_rule(or_relation->r2);
            start_nodes.push_back(sub_nodes.start_node);
            end_nodes.push_back(sub_nodes.end_node);

            return apply_or_rule(start_nodes, end_nodes);
        }
        else
        {
            // Apply AND rule
            const auto *and_relation = dynamic_cast<AndRelation *>(relation);

            start_and_end_nodes sub_nodes = apply_rule(and_relation->r1);
            start_nodes.push_back(sub_nodes.start_node);
            end_nodes.push_back(sub_nodes.end_node);
            sub_nodes = apply_rule(and_relation->r2);
            start_nodes.push_back(sub_nodes.start_node);
            end_nodes.push_back(sub_nodes.end_node);

            return apply_and_rule(start_nodes, end_nodes);
        }
    }
}

void NFA::construct_nfa()
{
    // Initialize the symbol_id_to_rule_index map
    for (int i=0; i<rules.size(); i++)
    {
        symbol_id_to_rule_index[rules[i].symbol->symbol_name] = i;
    }

    std::vector<NfaNode *> start_nodes, end_nodes;

    for (const auto &rule : rules)
    {
        symbol_id_to_start_and_end_node[rule.symbol->symbol_name] = apply_rule(rule.relation);
        NfaNode *start_node = symbol_id_to_start_and_end_node[rule.symbol->symbol_name].start_node;
        NfaNode *end_node = symbol_id_to_start_and_end_node[rule.symbol->symbol_name].end_node;

        start_nodes.push_back(start_node);
        end_nodes.push_back(end_node);

        end_node->token = Token(rule.token_index, rule.symbol->symbol_name);
    }
    if (start_nodes.size() > 1)
    {
        root = apply_or_rule(start_nodes, end_nodes).start_node;
    }
    else
    {
        root = start_nodes[0];
    }
}

NfaNode *NFA::get_root() const
{
    return root;
}