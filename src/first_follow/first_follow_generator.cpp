//
// Created by ryad on 23/12/24.
//

#include "first_follow_generator.h"
#include <algorithm>
#include <iostream>
#include <utility>

// Constructor
FirstFollowGenerator::FirstFollowGenerator(
    std::string  start_symbol,
    const std::set<std::string>& terminals,
    const std::set<std::string>& non_terminals,
    const std::vector<ParseRule>& parser_rules
) : startSymbol(std::move(start_symbol)), terminals(terminals), non_terminals(non_terminals), parser_rules(parser_rules) {
    for (const auto& rule : parser_rules) {
        lhs_to_parse_rule[rule.lhs] = rule;
    }
}

// Generate FIRST sets
void FirstFollowGenerator::generateFirst() {
    for (const auto& terminal : terminals) {
        std::set<std::string> tokens;
        tokens.insert(terminal);
        first_sets[terminal] = tokens;
    }
    for (const auto& rule : parser_rules) {
        computeFirst(rule);
    }
}

// Generate FOLLOW sets
void FirstFollowGenerator::generateFollow() {
    generateFirst();
    for (const auto& non_terminal : non_terminals) {
        computeFollow(non_terminal);
    }
}

// Getters
const std::map<std::string, std::set<std::string>>& FirstFollowGenerator::getFirstSets() const {
    return first_sets;
}

const std::map<std::string, std::set<std::string>>& FirstFollowGenerator::getFollowSets() const {
    return follow_sets;
}

// Helper to compute FIRST sets
void FirstFollowGenerator::computeFirst(const ParseRule& rule) {
    if (first_sets.find(rule.lhs) != first_sets.end()) {
        return;
    }
    std::set<std::string> tokens;
    for (const auto& productions : rule.or_expressions) {
        for (const auto& production : productions) {
            if (production.type == ParseUnitType::TERMINAL) {
                tokens.insert(production.name);
                if (production.name != "epslon")
                    break;
            }
            else {
                if (production.name == rule.lhs)
                    break;
                computeFirst(lhs_to_parse_rule[production.name]);
                for (auto& token : first_sets[production.name]) {
                    tokens.insert(token);
                }
                if (tokens.find("epslon") == tokens.end()) {
                    break;
                }
            }
        }
    }
    first_sets[rule.lhs] = tokens;
}

// Helper to compute FOLLOW sets
void FirstFollowGenerator::computeFollow(const std::string& non_terminal) {
    if (follow_sets.find(non_terminal) != follow_sets.end()) {
        return;
    }
    std::set<std::string> tokens;
    if (non_terminal == startSymbol) {
        tokens.insert("$");
    }
    for (const auto& rule : parser_rules) {
        for (const auto& productions : rule.or_expressions) {
            for (int i = 0; i < static_cast<int>(productions.size()); ++i) {
                if (productions[i].name == non_terminal) {
                    if (i == static_cast<int>(productions.size()) - 1 ) {
                        if (rule.lhs != non_terminal) {
                            computeFollow(rule.lhs);
                            for (auto& token : follow_sets[rule.lhs]) {
                                tokens.insert(token);
                            }
                        }
                        continue;
                    }
                    for (auto& token : first_sets[productions[i+1].name]) {
                        if (token != "epslon") {
                            tokens.insert(token);
                        }
                        else {
                             computeFollow(productions[i+1].name);
                             for (auto& token : follow_sets[productions[i+1].name]) {
                                 tokens.insert(token);
                             }
                        }
                    }
                }
            }
        }
    }
    follow_sets[non_terminal] = tokens;
}
