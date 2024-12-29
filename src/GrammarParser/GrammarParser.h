//
// Created by pc on 12/28/2024.
//

#ifndef COMPILER_PROJECT_GRAMMARPARSER_H
#define COMPILER_PROJECT_GRAMMARPARSER_H

#include <string>
#include <vector>
#include <set>
#include "../Parser/ParseStructs.h"

using  namespace std;

class GrammarParser {
    private:
        set<string> nonTerminals;
        set<string> terminals;
        vector<ParseRule> rules;
        int check_rules(string rule);
        int parseTerminals(string &rhs);
        int parseRules(string Terminal, string rhs);
    public:
        void parseGrammar(const string filePath);
        set<string> getNonTerminals();
        set<string> getTerminals();
        vector<ParseRule> getRules();
        string getStartSymbol();
};
#endif //COMPILER_PROJECT_GRAMMARPARSER_H
