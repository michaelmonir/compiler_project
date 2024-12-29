#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>

#include "RulesParser/structs.h"
#include "program.h"
#include "lexical_analyzer/lexical_analyzer.h"
#include "GrammarParser/GrammarParser.h"
#include "first_follow/first_follow_generator.h"


using namespace std;

//int main(int argc, char* argv[]) {
//        if (argc < 3) {
//            cerr << "Usage: " << argv[0] << " <file_name> <input_file_path>" << endl;
//            return 1;
//        }
//
//        string file_name = argv[1];      // First argument: DFA definition file
//        string input_file_path = argv[2]; // Second argument: input file path
//
//        // Read the DFA language input
//        vector<DfaNode*> start_nodes = read_language_input(file_name);
//
//        // Read input text from the specified file
//        ifstream input_file(input_file_path);
//
//        if (!input_file.is_open()) {
//            cerr << "Error: Could not open input file: " << input_file_path << endl;
//            return 1;
//        }
//
//        string input_text((istreambuf_iterator<char>(input_file)),
//                          (istreambuf_iterator<char>())); // Read entire file content
//        input_file.close();
//
//        // Perform lexical analysis
//        vector<LexemeClass> results = lexicalAnalyzer(input_text, start_nodes);
//
//        // Output the results
//        for (const LexemeClass& item : results) {
//            cout << item.lexemeClass << " " << item.lexeme << endl;
//        }
//
//        return 0;
//}

int main (){
    string file_name = "rules.txt";
    GrammarParser parser;
    parser.parseGrammar(file_name);

    // TODO: their is two problems in the following line, the start symbol what should it be?
    //  and the set of terminals and non terminals will be set parseUnit not strings,(i can change the is needed)
    //  this code also need to be tested
    FirstFollowGenerator generator(parser.getStartSymbol(), parser.getTerminals(), parser.getNonTerminals(), parser.getRules());

    return 0;
}
