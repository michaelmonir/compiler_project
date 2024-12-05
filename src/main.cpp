#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>

#include "RulesParser/structs.h"
#include "program.h"
#include "lexical_analyzer/lexical_analyzer.h"

using namespace std;

void printTransitionTable(const vector<DfaNode*> &start_nodes) {
    cout << "Transition Table:\n";
    for (DfaNode* node : start_nodes) {
        cout << "State " << node->dfa_node_index << " (" << node->token.token_name << "):\n";
        if (node->neighbors.empty()) {
            cout << "  No transitions\n";
        } else {
            for (const auto& transition : node->neighbors) {
                cout << "  Input: '" << transition.first << "' -> State "
                     << transition.second->dfa_node_index
                     << " (" << transition.second->token.token_name << ")\n";
            }
        }
        cout << endl;
    }
}

int main() {
    string file_name = "D:\\college terms\\term 7\\Compiler\\compiler_project\\test.txt";
    string input_text = "int sum , count , pass , mnt; while (pass !=10)"
                        "{"
                        "pass = pass + 1 ;"
                        "}";
    // string input_text = "";
    vector<DfaNode*> start_nodes = read_language_input(file_name);

    vector<LexemeClass> results = lexicalAnalyzer(input_text, start_nodes);


    for (LexemeClass item : results) {
        cout <<"lexeme: "<< item.lexeme << ", lexeme Class: " << item.lexemeClass << endl;
    }


    printTransitionTable(start_nodes);
    return 0;
}
