//
// Created by pc on 12/28/2024.
//

#ifndef COMPILER_PROJECT_GRAMMARPARSER_H
#define COMPILER_PROJECT_GRAMMARPARSER_H

#include <string>
#include <vector>
#include <set>

using  namespace std;
// Enumeration for parse_unit_type
enum class ParseUnitType {
    TERMINAL,
    NON_TERMINAL
};

// Structure for parse_unit
struct ParseUnit {
    string name;
    ParseUnitType type;
};

// Structure for parse_rule
struct ParseRule {
    string lhs;  // non_terminal lhs
    vector<vector<ParseUnit>> or_expressions;
};

class GrammarParser {
    private:
        set<string> nonTerminals;
        set<string> terminals;
        vector<ParseRule> rules;
        int check_rules(string rule);
        int parseTerminals(string rhs);
        int parseRules(string Terminal, string rhs);
    public:
        GrammarParser();
        void parseGrammar(const string filePath);
        set<string> getNonTerminals();
        set<string> getTerminals();
        vector<ParseRule> getRules();
        string getStartSymbol();
};
#endif //COMPILER_PROJECT_GRAMMARPARSER_H
