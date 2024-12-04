//
// Created by michael on 12/5/24.
//

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "../RulesParser/structs.h"
#include <iostream>
#include <vector>

using namespace std;

vector<LexemeClass> lexicalAnalyzer(string input, vector<DfaNode*> minimalDFA);

#endif //LEXICAL_ANALYZER_H
