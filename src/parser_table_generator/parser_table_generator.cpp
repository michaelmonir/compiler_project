//
// Created by mohamed arous on 12/24/2024.
//


#include "parser_table_generator.h"

#include "parser_table_generator.h"
#include <iostream>
#include <stdexcept>

// Helper to check for conflicts in the parse table
void addToParseTable(
    ParseTable& table,
    const std::string& non_terminal,
    const std::string& terminal,
    const std::vector<ParseUnit>& production
) {
    if (table[non_terminal].count(terminal)) {
        throw std::runtime_error("Conflict detected in the parse table for non-terminal: " + non_terminal + " with terminal: " + terminal);
    }
    table[non_terminal][terminal] = production;
}

// Generate LL(1) parse table
ParseTable generateParserTable(
    const std::map<std::string, std::set<std::string>>& first_sets,
    const std::map<std::string, std::set<std::string>>& follow_sets,
    const std::map<std::string, std::map<std::string, std::vector<ParseUnit>>> first_sets_with_productions,
    const std::vector<ParseRule>& parser_rules
) {
    ParseTable parse_table;

    for (const auto& rule : parser_rules) {
        const std::string& non_terminal = rule.lhs;

        // Iterate over the FIRST sets for this non-terminal
        for (const auto& [terminal, production] : first_sets_with_productions.at(non_terminal)) {
            if (terminal != "epslon") {
                addToParseTable(parse_table, non_terminal, terminal, production);
            }
        }

        // Handle epsilon productions
        if (first_sets_with_productions.at(non_terminal).count("epslon")) {
            for (const auto& terminal : follow_sets.at(non_terminal)) {
                addToParseTable(parse_table, non_terminal, terminal, {{"epslon", ParseUnitType::TERMINAL}});
            }
        }
    }

    return parse_table;
}

// Print the parse table
void printParserTable(const ParseTable& parse_table) {
    for (const auto& [non_terminal, row] : parse_table) {
        std::cout << "Non-terminal: " << non_terminal << "\n";
        for (const auto& [terminal, production] : row) {
            std::cout << "  Terminal: " << terminal << " -> ";
            for (const auto& unit : production) {
                std::cout << unit.name << " ";
            }
            std::cout << "\n";
        }
    }
}
