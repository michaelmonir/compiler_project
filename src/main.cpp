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
#include "Parser/ParcerStream.h"
#include "parser_table_generator/parser_table_generator.h"


using namespace std;

void phase2();

int main() {
//    if (argc < 3) {
//        cerr << "Usage: " << argv[0] << " <file_name> <input_file_path>" << endl;
//        return 1;
//    }

    // string file_name = "C:\\Users\\pc\\OneDrive-AlexandriaUniversity\\Documents\\GitHub\\compiler_project\\test.txt";      // First argument: DFA definition file
    // string input_file_path = "C:\\Users\\pc\\OneDrive-AlexandriaUniversity\\Documents\\GitHub\\compiler_project\\input.txt"; // Second argument: input file path

    string file_name = "/home/michael/mic/Projects/compiler_project2/compiler_project/test.txt";      // First argument: DFA definition file
    string input_file_path = "/home/michael/mic/Projects/compiler_project2/compiler_project/input.txt"; // Second argument: input file path

    // Read the DFA language input
    vector<DfaNode*> start_nodes = read_language_input(file_name);

    // Read input text from the specified file
    ifstream input_file(input_file_path);

    if (!input_file.is_open()) {
        cerr << "Error: Could not open input file: " << input_file_path << endl;
        return 1;
    }

    string input_text((istreambuf_iterator<char>(input_file)),
                      (istreambuf_iterator<char>())); // Read entire file content
    input_file.close();

    phase2();

    // Perform lexical analysis
    vector<LexemeClass> results = lexicalAnalyzer(input_text, start_nodes);

    // Output the results
    // for (const LexemeClass& item : results) {
    //     cout << item.lexemeClass << " " << item.lexeme << endl;
    // }

    empty_stack_after_last_token();
    left_most_print_root();
    return 0;
}

void phase2(){
    string file_name = "/home/michael/mic/Projects/compiler_project2/compiler_project/rules.txt";
    GrammarParser parser;
    parser.parseGrammar(file_name);

    // TODO: their is two problems in the following line, the start symbol what should it be?
    //  and the set of terminals and non terminals will be set parseUnit not strings,(i can change the is needed)
    //  this code also need to be tested
    // FirstFollowGenerator generator(parser.getStartSymbol(), parser.getTerminals(), parser.getNonTerminals(), parser.getRules());
    FirstFollowGenerator generator("METHOD_BODY", parser.getTerminals(), parser.getNonTerminals(), parser.getRules());
    generator.generateFirst();
    generator.generateFollow();
    ParseTable parse_table = generateParserTable(generator.getFirstSets(), generator.getFollowSets(), generator.getFirstSetsWithProductions(), parser.getRules());
    set<string> terminal_sets = parser.getTerminals();

    initialize_terminal_map_and_parse_table_adapt(terminal_sets, parse_table);
    ParseUnit unit;
    // unit.lhs = parser.getStartSymbol();
    unit.lhs = "METHOD_BODY";
    unit.type = ParseUnitType::NON_TERMINAL;
    initialize_stack(unit);
}
