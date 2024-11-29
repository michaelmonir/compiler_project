// //
// // Created by pc on 11/29/2024.
// //

#include "RulesParser.h"
#include <fstream>
#include <iostream>


int RulesParser::parseRules() {

    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error: could not open file" << endl;
        return -1;
    }

    string line;
    while(getline(file,line)){
        lineNumber++;
        trimString(line);
        TokenType type = getTokenType(line);

        switch (type) {
            case KEYWORD:
                parseKeywords(line);
                break;
            case PUNCTUATION:
                parsePunctuation(line);
                break;
            case REGULAR_DEFINITION:
                parseRegularDefinition(line);
                // todo: need to handel errors
                break;
            case REGULAR_EXPRESSION:
                parseRegularExpression(line);
                // todo: need to handel errors
                break;
            case UNDEFINED:
                cout << "Error: Undefined token at line " << lineNumber << endl;
                return -1;
        }

        file.close();

        for (const string& keyword: keywords){
            RegularExpression regularExpression(keyword, keyword, KEYWORD);
            regularExpressions.push_back(regularExpression);
        }


        for (const string& punc: punctuation){
            RegularExpression regularExpression(punc, punc, PUNCTUATION);
            regularExpressions.push_back(regularExpression);
        }

        sort(regularExpressions.begin(), regularExpressions.end(), [](RegularExpression a, RegularExpression b) {
            return a.getPriority() > b.getPriority();
        });

        return 0;
    }

    return 0;
}

TokenType RulesParser::getTokenType(string token) {
    if (token.front() == '{' && token.back() == '}')
        return KEYWORD;
    if (token.front() == '[' && token.back() == ']')
        return PUNCTUATION;
    if (isRegularDefinition(token))
        return REGULAR_DEFINITION;
    if (isRegularExpression(token))
        return REGULAR_EXPRESSION;
    return UNDEFINED;
}

bool RulesParser::isRegularExpression(string token) {
    size_t pos = token.find(':');
    return (pos != std::string::npos) && (pos == 0 || token[pos - 1] != '\\');
}

bool RulesParser::isRegularDefinition(string token) {
    size_t pos = token.find('=');
    return (pos != std::string::npos) && (pos == 0 || token[pos - 1] != '\\');
}

void RulesParser::parseKeywords(string token) {
    removeFirstAndLastChar(token);
    trimString(token);
    removeConsecutiveSpaces(token);
    vector<string> keywordsList = split(token, ' ');
    for (const string& keyword: keywordsList)
        keywords.push_back(keyword);
}

void RulesParser::parsePunctuation(string token) {
    removeFirstAndLastChar(token);
    trimString(token);
    removeConsecutiveSpaces(token);
    vector<string> punctuationList = split(token, ' ');
    for (const string& punc: punctuationList)
        punctuation.push_back(punc);
}

int RulesParser::parseRegularDefinition(string token) {
    removeConsecutiveSpaces(token);
    pair<string, string> definition = splitToken(token, '=');
    trimString(definition.first);
    trimString(definition.second);
    // todo: need to creat the RegularDefinition class
    // todo: need to add the definition to the regularDefinitions vector
    return 0;
}

int RulesParser::parseRegularExpression(string token) {
    removeConsecutiveSpaces(token);
    pair<string, string> definition = splitToken(token, ':');
    trimString(definition.first);
    trimString(definition.second);
    // todo: need to creat the RegularExpression class
    // todo: need to add the definition to the regularExpressions vector
    return 0;
}