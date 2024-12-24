//
// Created by michael on 12/5/24.
//
#include "lexical_analyzer.h"

map<string, string> symbolTable;

void panicRecovery(string::iterator &current, string::iterator end) {
    cout << "Error: Unrecognized token starting at \""
         << string(current, end) << "\"\n";

    // Skip until the next whitespace or end of input
    if (current != end ) {
        ++current;
    }
}

vector<LexemeClass> lexicalAnalyzer(string input, vector<DfaNode*> minimalDFA) {
    vector<LexemeClass> lexemes;
    auto current = input.begin();
    cout<<"symbol table "<<endl;
    while (current != input.end()) {
        if (isspace(*current)) {
            ++current;
            continue;
        }

        DfaNode* state = minimalDFA.front();
        string lexeme;
        auto lastAcceptingState = state;
        auto lastAcceptingPosition = current;
        while (state) {
            if (state->token.index != inf) { // Accepting state
                lastAcceptingState = state;
                lastAcceptingPosition = current;
            }
            state = state->neighbors[*current];
            if (current == input.end()) {
                break;
            }
            if (state) {
                lexeme += *current;
                ++current;
            }
        }
        if (lastAcceptingState->token.index != inf) {
            string lexemeClass = lastAcceptingState->token.token_name;
            if (lexemeClass == "id" && symbolTable.find(lexeme) == symbolTable.end()) {
                symbolTable[lexeme] = "id";
                cout <<"lexeme: "<< lexeme << ", lexeme Class: id"<< endl;
            }
            lexemes.push_back({lexemeClass, lexeme});
            if (current == input.end()) {
                return lexemes;
            }
            current = lastAcceptingPosition;
        } else {
            panicRecovery(current, input.end());
        }
    }

    return lexemes;
}
