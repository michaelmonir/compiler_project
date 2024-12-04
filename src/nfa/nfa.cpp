#include <vector>
#include <map>
#include <iostream>
#include "nfa.h"

// nfa_node constructor
nfa_node::nfa_node() : neighbors(257), is_final(false) {}

// NFA constructor
NFA::NFA(const std::vector<Rule> &rules) : rules(rules)
{
    construct_nfa();
}

start_and_end_nodes NFA::apply_char_rule(const CharRelation& relation)
{
    auto *starting_node = new nfa_node();
    auto *ending_node = new nfa_node();
    ending_node->is_final = true;
    starting_node->neighbors[relation.c].push_back(ending_node);
    const start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_or_rule(const std::vector<nfa_node *>& start_nodes, const std::vector<nfa_node *>& end_nodes)
{
    auto *starting_node = new nfa_node();
    auto *ending_node = new nfa_node();
    ending_node->is_final = true;

    // Connect all start nodes to a new starting node via epsilon transitions
    for (auto *start_node : start_nodes)
    {
        starting_node->neighbors[256].push_back(start_node); // epsilon transition
    }

    // Connect all end nodes to a new ending node via epsilon transitions
    for (auto *end_node : end_nodes)
    {
        end_node->is_final = false;
        end_node->neighbors[256].push_back(ending_node); // epsilon transition
    }

    start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_and_rule(const std::vector<nfa_node *> &start_nodes, const std::vector<nfa_node *> &end_nodes)
{
    auto *starting_node = new nfa_node();
    auto *ending_node = new nfa_node();
    ending_node->is_final = true;

    // Link the first start node to the first start node via epsilon transition
    starting_node->neighbors[256].push_back(start_nodes[0]);

    // Link intermediate end nodes to the next start nodes via epsilon transitions
    for (size_t i = 0; i < start_nodes.size() - 1; i++)
    {
        end_nodes[i]->is_final = false;
        end_nodes[i]->neighbors[256].push_back(start_nodes[i + 1]); // epsilon transition
    }

    // Link the last end node to the final node via epsilon transition
    end_nodes[start_nodes.size() - 1]->is_final = false;
    end_nodes[start_nodes.size() - 1]->neighbors[256].push_back(ending_node);

    start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_closure_rule(nfa_node *start_node, nfa_node *end_node)
{
    auto *starting_node = new nfa_node();
    auto *ending_node = new nfa_node();
    ending_node->is_final = true;
    end_node->is_final = false;

    end_node->neighbors[256].push_back(ending_node);     // epsilon transition
    starting_node->neighbors[256].push_back(start_node); // epsilon transition
    ending_node->neighbors[256].push_back(end_node);     // epsilon transition

    const start_and_end_nodes start_and_end_node = {starting_node, ending_node};
    return start_and_end_node;
}

start_and_end_nodes NFA::apply_symbol_rule(const Symbol *symbol)
{
    if (symbol_id_to_start_and_end_node.find(symbol->index) == symbol_id_to_start_and_end_node.end())
    {
        apply_rule(rules[symbol_id_to_rule_index[symbol->index]].relation);
    }

    start_and_end_nodes original_start_and_end_node = symbol_id_to_start_and_end_node[symbol->index];
    nfa_node *original_node = original_start_and_end_node.start_node;

    // Initialize the new end node pointer
    nfa_node *new_end_node = nullptr;

    // Start copying from the original start node
    nfa_node *new_start_node = deep_copy_tree(original_start_and_end_node.start_node, new_end_node);

    // Return the newly created start and end nodes
    return {new_start_node, new_end_node};
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
    }
    else if (dynamic_cast<ClosureRelation *>(relation))
    {
        // Apply closure rule (Kleene star)
        const auto *closure_relation = dynamic_cast<ClosureRelation *>(relation);
        auto [start_node, end_node] = apply_rule(closure_relation->relation);
        return apply_closure_rule(start_node, end_node);
    }
    else
    {
        // Handle OrRelation and AndRelation
        std::vector<nfa_node *> start_nodes, end_nodes;
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
        symbol_id_to_rule_index[rules[i].symbol->index] = i;
    }

    std::vector<nfa_node *> start_nodes, end_nodes;
    // Apply rules to construct the NFA
    for (const auto &rule : rules)
    {
        symbol_id_to_start_and_end_node[rule.symbol->index] = apply_rule(rule.relation);
        start_nodes.push_back(symbol_id_to_start_and_end_node[rule.symbol->index].start_node);
        end_nodes.push_back(symbol_id_to_start_and_end_node[rule.symbol->index].end_node);
    }
    if (start_nodes.size() > 1)
    {
        root = apply_or_rule(start_nodes, end_nodes).start_node;
    }
    else
    {
        root =start_nodes[0];
    }
}

nfa_node *NFA::get_root() const
{
    return root;
}

nfa_node *NFA::deep_copy_tree(nfa_node *original_node, nfa_node *&new_end_node)
{
    if (original_node == nullptr)
        return nullptr;

    // Create a new node
    auto *new_node = new nfa_node();
    new_node->is_final = original_node->is_final;

    // Copy neighbors recursively
    for (int i = 0; i < original_node->neighbors.size(); ++i)
    {
        for (auto *neighbor : original_node->neighbors[i])
        {
            new_node->neighbors[i].push_back(deep_copy_tree(neighbor, new_end_node));
        }
    }

    // If this is the original end node, update the new end node pointer
    if (original_node->is_final)
    {
        new_end_node = new_node;
    }

    return new_node;
}
