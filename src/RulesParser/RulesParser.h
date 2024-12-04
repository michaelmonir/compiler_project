//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_RULESPARSER_H
#define COMPILER_PROJECT_RULESPARSER_H

#include <string>
#include <vector>

using namespace std;

#include "string_to_relation_converter.h"

enum TokenType {
    KEYWORD,
    PUNCTUATION,
    REGULAR_DEFINITION,
    REGULAR_EXPRESSION,
    UNDEFINED
};

class RegularDefinition {
public:
    string symbol_name;
    Relation* relation;

    RegularDefinition(string name, Relation* relation)
        : symbol_name(name), relation(relation) {};
};

class RegularExpression {
public:
    string symbol_name;
    Relation* relation;
    int priority;
    RegularExpression(string name, Relation* relation, int priority)
        : symbol_name(name), relation(relation), priority(priority) {};
};

vector<Rule> parseRules(string filepath);

#endif //COMPILER_PROJECT_RULESPARSER_H
