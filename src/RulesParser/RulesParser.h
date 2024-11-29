//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_RULESPARSER_H
#define COMPILER_PROJECT_RULESPARSER_H

#include <string>
#include <vector>

using namespace std;

enum TokenType {
    KEYWORD,
    PUNCTUATION,
    REGULAR_DEFINITION,
    REGULAR_EXPRESSION,
    UNDEFINED
};

class RegularDefinition {
};
class RegularExpression {
private:
    string name;
    string regex;
    TokenType priority;
public:
    TokenType getPriority() { return priority; }
    RegularExpression(string name, string regex, TokenType priority) : name(name), regex(regex), priority(priority) {};
};

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
