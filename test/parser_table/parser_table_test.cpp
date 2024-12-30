//
// Created by mohamed arous on 12/24/2024.
//
#include <gtest/gtest.h>
#include "../../src/parser_table_generator/parser_table_generator.h"
#include "parser_table_generator/parser_table_generator.h"

// Helper function to compare productions
bool areProductionsEqual(const std::vector<ParseUnit>& prod1, const std::vector<ParseUnit>& prod2) {
    if (prod1.size() != prod2.size()) return false;

    for (size_t i = 0; i < prod1.size(); ++i) {
        if (!(prod1[i].lhs == prod2[i].lhs)) return false;
    }
    return true;
}


// Test for Simple Grammar
// TEST(ParserTableTest, SimpleGrammar) {
//     std::set<std::string> terminals = {"a", "b", "\\L"};
//     std::set<std::string> non_terminals = {"S", "A", "B"};
//     std::string start_symbol = "S";
//
//     std::vector<ParseRule> rules = {
//         {"S", {{{"A", ParseUnitType::NON_TERMINAL}, {"B", ParseUnitType::NON_TERMINAL}}}},
//         {"A", {{{"a", ParseUnitType::TERMINAL}}, {{"\\L", ParseUnitType::TERMINAL}}}},
//         {"B", {{{"b", ParseUnitType::TERMINAL}}}}
//     };
//
//     // Mock FIRST sets with productions
//     std::map<std::string, std::map<std::string, std::vector<ParseUnit>>> first_sets_with_productions = {
//         {"S", {{"a", {{"A", ParseUnitType::NON_TERMINAL}, {"B", ParseUnitType::NON_TERMINAL}}},
//                 {"b", {{"A", ParseUnitType::NON_TERMINAL}, {"B", ParseUnitType::NON_TERMINAL}}}}},
//         {"A", {{"a", {{"a", ParseUnitType::TERMINAL}}},
//                 {"\\L", {}}}},
//         {"B", {{"b", {{"b", ParseUnitType::TERMINAL}}}}}
//     };
//
//     // Mock FOLLOW sets
//     std::map<std::string, std::set<std::string>> follow_sets = {
//         {"S", {"$"}},
//         {"A", {"b"}},
//         {"B", {"$"}}
//     };
//
//     // Generate the parse table
//     ParseTable parse_table = generateParserTable({}, follow_sets, first_sets_with_productions, rules);
//
//     // Expected parse table
//     // Expected parse table
//     ParseTable expected_table = {
//         {"S", {
//             {"a", {{"A", ParseUnitType::NON_TERMINAL}, {"B", ParseUnitType::NON_TERMINAL}}},
//                {"b", {{"A", ParseUnitType::NON_TERMINAL}, {"B", ParseUnitType::NON_TERMINAL}}}
//         }},
//         {"A", {
//             {"a", {{"a", ParseUnitType::TERMINAL}}},
//             {"b", {{"\\L", ParseUnitType::TERMINAL}}}
//         }},
//         {"B", {
//             {"b", {{"b", ParseUnitType::TERMINAL}}}
//         }}
//     };
//
//
//     // Validate the parse table
//     std::cout<<"hello"<< std::endl;
//     ASSERT_TRUE(parse_table.size() ==  expected_table.size());
//     for (const auto& [non_terminal, entries] : expected_table) {
//         ASSERT_TRUE(parse_table[non_terminal].size() == entries.size());
//         for (const auto& [terminal, production] : entries) {
//             ASSERT_TRUE(areProductionsEqual(parse_table[non_terminal][terminal], production));
//         }
//     }
// }

// Test for Conflict Detection
// TEST(ParserTableTest, ConflictDetection) {
//     std::set<std::string> terminals = {"a", "b"};
//     std::set<std::string> non_terminals = {"S"};
//     std::string start_symbol = "S";
//
//     std::vector<ParseRule> rules = {
//         {"S", {{{"a", ParseUnitType::TERMINAL}}, {{"b", ParseUnitType::TERMINAL}}}}
//     };
//
//     // Mock FIRST sets with conflicts
//     std::map<std::string, std::map<std::string, std::vector<ParseUnit>>> first_sets_with_productions = {
//         {"S", {{"a", {{"a", ParseUnitType::TERMINAL}}},
//                 {"a", {{"b", ParseUnitType::TERMINAL}}}}}
//     };
//
//     // Mock FOLLOW sets
//     std::map<std::string, std::set<std::string>> follow_sets = {
//         {"S", {"$"}}
//     };
//
//     // Expect runtime error due to conflict
//     EXPECT_THROW(generateParserTable({}, follow_sets, first_sets_with_productions, rules), std::runtime_error);
// }

// // Test for Recursive Grammar
// TEST(ParserTableTest, RecursiveGrammar) {
//     std::set<std::string> terminals = {"+", "*", "(", ")", "id"};
//     std::set<std::string> non_terminals = {"E", "E'", "T", "T'", "F"};
//     std::string start_symbol = "E";
//
//     std::vector<ParseRule> rules = {
//         {"E", {{{"T", ParseUnitType::NON_TERMINAL}, {"E'", ParseUnitType::NON_TERMINAL}}}},
//         {"E'", {{{"+", ParseUnitType::TERMINAL}, {"T", ParseUnitType::NON_TERMINAL}, {"E'", ParseUnitType::NON_TERMINAL}}, {}}}},
//             {"T", {{{"F", ParseUnitType::NON_TERMINAL}, {"T'", ParseUnitType::NON_TERMINAL}}}},
//         {"T'", {{{"*", ParseUnitType::TERMINAL}, {"F", ParseUnitType::NON_TERMINAL}, {"T'", ParseUnitType::NON_TERMINAL}}, {}}}},
//         {"F", {{{"(", ParseUnitType::TERMINAL}, {"E", ParseUnitType::NON_TERMINAL}, {")", ParseUnitType::TERMINAL}},
//                {{"id", ParseUnitType::TERMINAL}}}}
//     };
//
//     // Mock FIRST sets with productions
//     std::map<std::string, std::map<std::string, std::vector<ParseUnit>>> first_sets_with_productions = {
//         {"E", {{"(", {{"T", ParseUnitType::NON_TERMINAL}, {"E'", ParseUnitType::NON_TERMINAL}}},
//                 {"id", {{"T", ParseUnitType::NON_TERMINAL}, {"E'", ParseUnitType::NON_TERMINAL}}}}},
//         {"E'", {{"+", {{"+", ParseUnitType::TERMINAL}, {"T", ParseUnitType::NON_TERMINAL}, {"E'", ParseUnitType::NON_TERMINAL}}},
//                  {"\\L", {}}}},
//         {"T", {{"(", {{"F", ParseUnitType::NON_TERMINAL}, {"T'", ParseUnitType::NON_TERMINAL}}},
//                 {"id", {{"F", ParseUnitType::NON_TERMINAL}, {"T'", ParseUnitType::NON_TERMINAL}}}}},
//         {"T'", {{"*", {{"*", ParseUnitType::TERMINAL}, {"F", ParseUnitType::NON_TERMINAL}, {"T'", ParseUnitType::NON_TERMINAL}}},
//                  {"\\L", {}}}},
//         {"F", {{"(", {{"(", ParseUnitType::TERMINAL}, {"E", ParseUnitType::NON_TERMINAL}, {")", ParseUnitType::TERMINAL}}},
//                 {"id", {{"id", ParseUnitType::TERMINAL}}}}}
//     };
//
//     // Mock FOLLOW sets
//     std::map<std::string, std::set<std::string>> follow_sets = {
//         {"E", {"$", ")"}},
//         {"E'", {"$", ")"}},
//         {"T", {"+", "$", ")"}},
//         {"T'", {"+", "$", ")"}},
//         {"F", {"*", "+", "$", ")"}}
//     };
//
//     // Generate the parse table
//     ParseTable parse_table = generateParserTable({}, follow_sets, first_sets_with_productions, rules);
//
//     // Validate specific entries in the parse table
//     ASSERT_TRUE(parse_table["E"].count("(") > 0);
//     ASSERT_TRUE(parse_table["E"].count("id") > 0);
//     ASSERT_TRUE(parse_table["E'"]["+"].size() > 0);
//     ASSERT_TRUE(parse_table["T'"]["*"].size() > 0);
//     ASSERT_TRUE(parse_table["F"].count("(") > 0);
// }


