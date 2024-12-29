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

const std::map<std::string, std::map<std::string, std::vector<ParseUnit> > > &FirstFollowGenerator::getFirstSetsWithProductions() const {
    return first_sets_with_productions;
}


// Helper to compute FIRST sets
void FirstFollowGenerator::computeFirst(const ParseRule& rule) {
    if (first_sets.find(rule.lhs) != first_sets.end()) {
        return;
    }
    std::set<std::string> tokens;
    std::map<std::string, std::vector<ParseUnit>> token_to_production;
    for (const auto& productions : rule.or_expressions) {
        for (const auto& production : productions) {
            if (production.type == ParseUnitType::TERMINAL) {
                tokens.insert(production.lhs);
                token_to_production[production.lhs] = productions;
                if (production.lhs != "epslon")
                    break;
            }
            else {
                if (production.lhs == rule.lhs)
                    break;
                computeFirst(lhs_to_parse_rule[production.lhs]);
                for (auto& token : first_sets[production.lhs]) {
                    tokens.insert(token);
                    token_to_production[token] = productions;
                }
                if (tokens.find("epslon") == tokens.end()) {
                    break;
                }
            }
        }
    }
    first_sets[rule.lhs] = tokens;
    first_sets_with_productions[rule.lhs] = token_to_production;
}

// Helper to compute FOLLOW sets
void FirstFollowGenerator::computeFollow(const std::string& non_terminal) {
    if (follow_sets.find(non_terminal) != follow_sets.end() || visited_non_terminals.find(non_terminal) != visited_non_terminals.end()) {
        return;
    }
    visited_non_terminals.insert(non_terminal);
    std::set<std::string> tokens;
    if (non_terminal == startSymbol) {
        tokens.insert("$");
    }
    for (const auto& rule : parser_rules) {
        for (const auto& productions : rule.or_expressions) {
            for (int i = 0; i < static_cast<int>(productions.size()); ++i) {
                if (productions[i].lhs == non_terminal) {
                    if (i == static_cast<int>(productions.size()) - 1 ) {
                        computeFollow(rule.lhs);
                        for (auto& token : follow_sets[rule.lhs]) {
                            tokens.insert(token);
                        }
                        continue;
                    }
                    for (auto& token : first_sets[productions[i+1].lhs]) {
                        if (token != "epslon") {
                            tokens.insert(token);
                        }
                        else {
                             computeFollow(productions[i+1].lhs);
                             for (auto& token : follow_sets[productions[i+1].lhs]) {
                                 tokens.insert(token);
                             }
                        }
                    }
                }
            }
        }
    }
    follow_sets[non_terminal] = tokens;
    visited_non_terminals.erase(non_terminal);
}
