//
// Created by ryad on 04/12/24.
//

#include <gtest/gtest.h>
#include <nfa/nfa.h>

#include "../../src/RulesParser/RulesParser.h"

Rule create_literal_rule(const char literal, const int symbol_index)
{
    auto *symbol = new Symbol(symbol_index, "literal");
    Relation *relation = new CharRelation(literal);
    return {symbol, relation, inf};
}

// Test helper: Create a simple OR rule
Rule create_or_rule(const vector<Relation *> &children, const int Symbol_index)
{
    auto *symbol = new Symbol(Symbol_index, "or");
    auto *relation = new OrRelation(children[0], children[1]);
    return {symbol, relation, inf};
}

// Test helper: Create a simple AND rule
Rule create_and_rule(const vector<Relation *> &children, const int Symbol_index)
{
    auto *symbol = new Symbol(Symbol_index, "and");
    auto *relation = new AndRelation(children[0], children[1]);
    return {symbol, relation, inf};
}

// Test NFA construction for a single literal rule
TEST(NFATest, SingleLiteralRule)
{
    vector<Rule> rules;
    rules.push_back(create_literal_rule('a', 0));

    const NFA nfa(rules);

    auto *root = nfa.get_root();
    ASSERT_NE(root, nullptr);

    ASSERT_EQ(root->neighbors[256].size(), 0); // No epsilon transitions
    ASSERT_EQ(root->neighbors['a'].size(), 1); // One transition for 'a'
    auto *next_node = root->neighbors['a'][0];
    ASSERT_NE(next_node, nullptr);
    ASSERT_TRUE(next_node->is_final); // Final node
}

// Test NFA construction for an OR rule
TEST(NFATest, OrRule)
{
    vector<Rule> rules;
    Relation *child1 = new CharRelation('a');
    Relation *child2 = new CharRelation('b');
    rules.push_back(create_or_rule({child1, child2}, 0));

    const NFA nfa(rules);

    auto *root = nfa.get_root();
    ASSERT_NE(root, nullptr);

    ASSERT_EQ(root->neighbors[256].size(), 2); // epsilon links to the two children

    const auto *child_a = root->neighbors[256][0];
    const auto *child_b = root->neighbors[256][1];

    ASSERT_EQ(child_a->neighbors['a'].size(), 1);
    ASSERT_EQ(child_b->neighbors['b'].size(), 1);

    ASSERT_TRUE(child_a->neighbors['a'][0]->neighbors[256][0]->is_final);
    ASSERT_TRUE(child_b->neighbors['b'][0]->neighbors[256][0]->is_final);
}

// Test NFA construction for an AND rule
TEST(NFATest, AndRule)
{
    vector<Rule> rules;
    Relation *child1 = new CharRelation('a');
    Relation *child2 = new CharRelation('b');
    rules.push_back(create_and_rule({child1, child2}, 0));

    const NFA nfa(rules);

    auto *root = nfa.get_root();
    ASSERT_NE(root, nullptr);

    ASSERT_EQ(root->neighbors[256].size(), 1); // epsilon link to first child

    const auto *first = root->neighbors[256][0];
    ASSERT_EQ(first->neighbors['a'].size(), 1);

    const auto *second = first->neighbors['a'][0]->neighbors[256][0];
    ASSERT_EQ(second->neighbors['b'].size(), 1);

    ASSERT_TRUE(second->neighbors['b'][0]->neighbors[256][0]->is_final);
}

// Test NFA construction for a closure (Kleene star)
TEST(NFATest, ClosureRule)
{
    vector<Rule> rules;
    Relation *child = new CharRelation('a');
    Relation *closure = new ClosureRelation(child, true); // assuming positive closure
    auto *symbol = new Symbol(0, "closure");
    rules.emplace_back(symbol, closure, inf);

    const NFA nfa(rules);

    auto *root = nfa.get_root();
    ASSERT_NE(root, nullptr);

    ASSERT_EQ(root->neighbors[256].size(), 1); // epsilon link to the closure start

    auto *closure_start = root->neighbors[256][0];
    ASSERT_EQ(closure_start->neighbors['a'].size(), 1);

    const auto *closure_end = closure_start->neighbors['a'][0];
    ASSERT_EQ(closure_end->neighbors[256].size(), 1);
    ASSERT_TRUE(closure_end->neighbors[256][0]->is_final);
}

// Test NFA construction for mixed rules (AND + OR)
TEST(NFATest, MixedRules)
{
    vector<Rule> rules;
    Relation *lit1 = new CharRelation('a');
    Relation *lit2 = new CharRelation('b');
    Relation *lit3 = new CharRelation('c');

    Relation *and_relation = new AndRelation(lit1, lit2);

    Relation *or_relation = new OrRelation(and_relation, lit3);

    auto *symbol = new Symbol(0, "mixed");
    rules.emplace_back(symbol, or_relation, inf);

    const NFA nfa(rules);

    auto *root = nfa.get_root();
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->neighbors[256].size(), 2); // OR rule branches
}
