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

    bool operator<(const ParseUnit& other) const {
        return name < other.name;  // Compare by name
    }

    ParseUnit() = default;

    ParseUnit(string it, ParseUnitType cond) {
        name = it;
        type = cond;
    }

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

    ParseTableItem() = default;

    ParseTableItem(bool first, const pair<bool, vector<ParseUnit>>::second_type & vector) {
        isSync = first;
        rhs = vector;    }
};

#endif //PARSESTRUCTS_H
