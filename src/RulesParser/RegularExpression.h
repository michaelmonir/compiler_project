//
// Created by pc on 11/29/2024.
//

#ifndef COMPILER_PROJECT_REGULAREXPRESSION_H
#define COMPILER_PROJECT_REGULAREXPRESSION_H

#include "RegularDefinition.h"
#include "TokenType.h"

using namespace std;

class RegularExpression {
    private:
        string name;
        string regex;
        TokenType priority;
    public:
        TokenType getPriority() { return priority; }
        RegularExpression(string name, string regex, TokenType priority) : name(name), regex(regex), priority(priority) {};
};

#endif //COMPILER_PROJECT_REGULAREXPRESSION_H
