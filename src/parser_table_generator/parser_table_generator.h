//
// Created by mohamed arous on 12/24/2024.
//

#ifndef PARSER_TABLE_GENERATOR_H
#define PARSER_TABLE_GENERATOR_H

#include <string>
#include <vector>
#include <map>
#include <set>

// Enum for parse unit types
enum class ParseUnitType {
    TERMINAL,
    NON_TERMINAL
};

// Structure representing a parse unit
struct ParseUnit {
    std::string name;
    ParseUnitType type;
};

// Structure representing a parser rule
struct ParseRule {
    std::string lhs; // Non-terminal on the left-hand side
    std::vector<std::vector<ParseUnit>> or_expressions; // Right-hand side expressions
};

// Type alias for the parse table
using ParseTable = std::map<std::string, std::map<std::string, std::pair<bool, std::vector<ParseUnit>>>>;

// Functions
ParseTable generateParserTable(
    const std::map<std::string, std::set<std::string>>& first_sets,
    const std::map<std::string, std::set<std::string>>& follow_sets,
    const std::map<std::string, std::map<std::string, std::vector<ParseUnit>>> first_sets_with_productions,
    const std::vector<ParseRule>& parser_rules
);


void printParserTable(const ParseTable& parse_table);

#endif // PARSER_TABLE_H
