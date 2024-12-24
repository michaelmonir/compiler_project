//
// Created by michael on 12/24/24.
//

#ifndef PARSESTRUCTS_H
#define PARSESTRUCTS_H

#include "string"
#include "vector"
#include "../RulesParser/structs.h"

using namespace std;

enum ParseUnitType {
    TERMINAL,
    NON_TERMINAL
};

class ParseUnit{
public:
    string name;
    ParseUnitType type;
};

class ParseRule {
public:
    ParseUnit lhs;
    vector<vector<ParseUnit>> or_expressions;
};

class ParseTableItem {
public:
    bool isSync;
    vector<ParseUnit> rhs;
};

#endif //PARSESTRUCTS_H
