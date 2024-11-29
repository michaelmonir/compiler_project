//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_RULESPARSER_H
#define COMPILER_PROJECT_RULESPARSER_H

#include <string>
#include <vector>
#include "RegularExpression.h"

using namespace std;

class RulesParser {
    private:
          string filePath;
          int lineNumber;
          vector<RegularExpression> regularExpressions;
          vector<RegularDefinition> regularDefinitions;
          vector<string> punctuation;
          vector<string> keywords;
    public:
        RulesParser(string filePath);
        enum TokenType {
            KEYWORD,
            PUNCTUATION,
            REGULAR_DEFINITION,
            REGULAR_EXPRESSION
        };
        int parseRules();

};
#endif //COMPILER_PROJECT_RULESPARSER_H
