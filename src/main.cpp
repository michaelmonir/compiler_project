#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include "dfa_conversion/dfa_convertor.h"

class DfaNode;
using namespace std;

struct LexemeClass {
    string lexemeClass;
    string lexeme;
};

map<string, string> symbolTable;
void panicRecovery(string::iterator &current, string::iterator end) {
    cout << "Error: Unrecognized token starting at \""
         << string(current, end) << "\"\n";

    // Skip until the next whitespace or end of input
    while (current != end && !isspace(*current)) {
        ++current;
    }
}

vector<LexemeClass> lexicalAnalyzer(string input, vector<DfaNode*> minimalDFA) {
    vector<LexemeClass> lexemes;
    auto current = input.begin();
    while (current != input.end()) {
        if (isspace(*current)) {
            ++current;
            continue;
        }


        DfaNode* state = minimalDFA.front();
        string lexeme;
        auto lastAcceptingState = state;
        auto lastAcceptingPosition = current;
        while (state && current != input.end()) {
            if (state->token.index != inf) { // Accepting state
                lastAcceptingState = state;
                lastAcceptingPosition = current;
            }
            state = state->neighbors[*current];
            if (state) {
                lexeme += *current;
                ++current;
            }
        }
        if (lastAcceptingState->token.index != inf) {
            string lexemeClass = lastAcceptingState->token.token_name;
            if (lexemeClass == "ID" && symbolTable.find(lexeme) == symbolTable.end()) {
                symbolTable[lexeme] = "ID";
            }
            lexemes.push_back({lexemeClass, lexeme});
            current = lastAcceptingPosition;
        } else {
            panicRecovery(current, input.end());
        }
    }

    return lexemes;
}

int main() {
    cout<<"hello world"<<endl;

    return 0;
}
