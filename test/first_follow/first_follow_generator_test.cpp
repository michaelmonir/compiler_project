//
// Created by ryad on 24/12/24.
//

#include <gtest/gtest.h>
#include "../../src/first_follow/first_follow_generator.h"

// Helper function to compare sets
bool areSetsEqual(const std::set<std::string>& set1, const std::set<std::string>& set2) {
    return set1 == set2;
}

// Test for Simple Grammar
TEST(FirstFollowGeneratorTest, SimpleGrammar) {
    std::set<std::string> terminals = {"a", "b", "epslon"};
    std::set<std::string> non_terminals = {"S", "A", "B"};
    std::string start_symbol = "S";

    std::vector<ParseRule> rules = {
        {"S", {{{"A", ParseUnitType::NON_TERMINAL}, {"B", ParseUnitType::NON_TERMINAL}}}},
        {"A", {{{"a", ParseUnitType::TERMINAL}}, {{"epslon", ParseUnitType::TERMINAL}}}},
        {"B", {{{"b", ParseUnitType::TERMINAL}}}}
    };

    FirstFollowGenerator generator(start_symbol, terminals, non_terminals, rules);
    generator.generateFirst();
    generator.generateFollow();

    // Expected FIRST sets
    std::map<std::string, std::set<std::string>> expected_first = {
        {"S", {"a", "b", "epslon"}},
        {"A", {"a", "epslon"}},
        {"B", {"b"}}
    };

    // Expected FOLLOW sets
    std::map<std::string, std::set<std::string>> expected_follow = {
        {"S", {"$"}},
        {"A", {"b"}},
        {"B", {"$"}}
    };

    // Check FIRST sets
    for (const auto& [non_terminal, first_set] : expected_first) {
        ASSERT_TRUE(areSetsEqual(first_set, generator.getFirstSets().at(non_terminal)));
    }

    // Check FOLLOW sets
    for (const auto& [non_terminal, follow_set] : expected_follow) {
        ASSERT_TRUE(areSetsEqual(follow_set, generator.getFollowSets().at(non_terminal)));
    }
}

// Test for Recursive Grammar
TEST(FirstFollowGeneratorTest, RecursiveGrammar) {
    std::set<std::string> terminals = {"+", "*", "(", ")", "id"};
    std::set<std::string> non_terminals = {"E", "T", "F"};
    std::string start_symbol = "E";

    std::vector<ParseRule> rules = {
        {"E", {{{"E", ParseUnitType::NON_TERMINAL}, {"+", ParseUnitType::TERMINAL}, {"T", ParseUnitType::NON_TERMINAL}},
               {{"T", ParseUnitType::NON_TERMINAL}}}},
        {"T", {{{"T", ParseUnitType::NON_TERMINAL}, {"*", ParseUnitType::TERMINAL}, {"F", ParseUnitType::NON_TERMINAL}},
               {{"F", ParseUnitType::NON_TERMINAL}}}},
        {"F", {{{"(", ParseUnitType::TERMINAL}, {"E", ParseUnitType::NON_TERMINAL}, {")", ParseUnitType::TERMINAL}},
               {{"id", ParseUnitType::TERMINAL}}}}
    };

    FirstFollowGenerator generator(start_symbol, terminals, non_terminals, rules);
    generator.generateFirst();
    generator.generateFollow();

    // Expected FIRST sets
    std::map<std::string, std::set<std::string>> expected_first = {
        {"+", {"+"}},
        {"*", {"*"}},
        {"id", {"id"}},
        {"(", {"("}},
        {")", {")"}},
        {"E", {"(", "id"}},
        {"T", {"(", "id"}},
        {"F", {"(", "id"}}
    };

    // Expected FOLLOW sets
    std::map<std::string, std::set<std::string>> expected_follow = {
        {"E", {")", "$", "+"}},
        {"T", {")", "$", "+", "*"}},
        {"F", {")", "$", "+", "*"}}
    };

    // Check FIRST sets
    for (const auto& [non_terminal, first_set] : expected_first) {
        ASSERT_TRUE(areSetsEqual(first_set, generator.getFirstSets().at(non_terminal)));
    }

    // Check FOLLOW sets
    for (const auto& [non_terminal, follow_set] : expected_follow) {
        ASSERT_TRUE(areSetsEqual(follow_set, generator.getFollowSets().at(non_terminal)));
    }
}

