//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_RULESPARSER_H
#define COMPILER_PROJECT_RULESPARSER_H

#include <string>
#include <vector>
#include "RegularExpression.h"
#include "stringUtils.cpp"

using namespace std;

class RulesParser {
    private:
          string filePath;
          int lineNumber;
          vector<RegularExpression> regularExpressions;
          vector<RegularDefinition> regularDefinitions;
          vector<string> punctuation;
          vector<string> keywords;
          TokenType getTokenType(string token);
          bool isRegularExpression(string token);
          bool isRegularDefinition(string token);
          void parseKeywords(string token);
          void parsePunctuation(string token);
          int parseRegularDefinition(string token);
          int parseRegularExpression(string token);
    public:
        RulesParser(string filePath) : filePath(filePath), lineNumber(0) {};
        int parseRules();
};
#endif //COMPILER_PROJECT_RULESPARSER_H
