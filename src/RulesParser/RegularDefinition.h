//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_REGULARDEFINITION_H
#define COMPILER_PROJECT_REGULARDEFINITION_H

#include <string>
#include "stringUtils.h"

using namespace std;

class RegularDefinition {
    private:
        string name;
        string regex;
        int expandRange(string &regex);
        void addParentheses(string &regex);
        void processAccumulator(vector<char> &accumulator, vector<char> &result, const unordered_map<string, string> &definitionMap);
        void expandDefinitions(string &regex, const vector<RegularDefinition> &regularDefinitions);
        string parenthesizeDisjunctions(std::string &str);
public:
        string getName() { return name; }
        string getRegex() { return regex; }
        int formalizeRegex(vector<RegularDefinition> regularDefinitions);
        RegularDefinition(string name, string regex) : name(replaceAll(name, "\\", "")), regex(regex) {}
};

#endif //COMPILER_PROJECT_REGULARDEFINITION_H
