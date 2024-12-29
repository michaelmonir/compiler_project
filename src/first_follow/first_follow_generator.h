//
// Created by ryad on 23/12/24.
//

#ifndef FIRST_FOLLOW_GENERATOR_H
#define FIRST_FOLLOW_GENERATOR_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "../GrammarParser/GrammarParser.h"


class FirstFollowGenerator {
private:
    std::string startSymbol;
    std::set<std::string> terminals;
    std::set<std::string> non_terminals;
    std::vector<ParseRule> parser_rules;
    std::map<std::string, ParseRule> lhs_to_parse_rule;
    std::map<std::string, std::set<std::string>> first_sets;
    std::map<std::string, std::set<std::string>> follow_sets;

    void computeFirst(const ParseRule& rule);
    void computeFollow(const std::string& non_terminal);

public:
    FirstFollowGenerator(
        std::string  start_symbol,
        const std::set<std::string>& terminals,
        const std::set<std::string>& non_terminals,
        const std::vector<ParseRule>& parser_rules
    );

    void generateFirst();
    void generateFollow();

    const std::map<std::string, std::set<std::string>>& getFirstSets() const;
    const std::map<std::string, std::set<std::string>>& getFollowSets() const;
};

#endif //FIRST_FOLLOW_GENERATOR_H
