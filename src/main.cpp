#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>

#include "RulesParser/structs.h"
#include "program.h"
#include "lexical_analyzer/lexical_analyzer.h"

using namespace std;

int main(int argc, char* argv[]) {
//    string file_name = "C:\\Users\\pc\\OneDrive-AlexandriaUniversity\\Documents\\GitHub\\compiler_project\\test.txt";
//    string input_text = "int main() {\n"
//                        "    float x = 3;\n"
//                        "    int y = 42;\n"
//                        "    boolean flag = true;\n"
//                        "\n"
//                        "    if (x >= y) {\n"
//                        "        x = x + y * 2;\n"
//                        "    } else {\n"
//                        "        y = y - x / 3;\n"
//                        "    }"
//                        "    while (flag) {"
//                        "        x = x - 1;"
//                        "        if (x == 0 || x < 0) {\n"
//                        "            flag = false;\n"
//                        "        }\n"
//                        "    }\n"
//                        "}";
//    // string input_text = "";
//    vector<DfaNode*> start_nodes = read_language_input(file_name);

        if (argc < 3) {
            cerr << "Usage: " << argv[0] << " <file_name> <input_file_path>" << endl;
            return 1;
        }

        string file_name = argv[1];      // First argument: DFA definition file
        string input_file_path = argv[2]; // Second argument: input file path

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

        // Perform lexical analysis
        vector<LexemeClass> results = lexicalAnalyzer(input_text, start_nodes);

        // Output the results
        for (const LexemeClass& item : results) {
            cout << item.lexemeClass << " " << item.lexeme << endl;
        }

        return 0;
    }
