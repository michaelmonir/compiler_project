//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_REGULAREXPRESSION_H
#define COMPILER_PROJECT_REGULAREXPRESSION_H

#include "RegularDefinition.h"
#include "TokenType.h"

using namespace std;

class RegularExpression : public RegularDefinition {
    private:

        int priority;
    public:
        int getPriority() { return priority; }
        RegularExpression(string name, string regex, int priority)
            : RegularDefinition(name, regex), priority(priority) {}
        string toString() {
            return "Name: " + getName() + ", Regex: " + getRegex() + ", Priority: " + to_string(priority);}
};

#endif //COMPILER_PROJECT_REGULAREXPRESSION_H
