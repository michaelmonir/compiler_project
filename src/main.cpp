#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>

#include "RulesParser/structs.h"
#include "program.h"
#include "lexical_analyzer/lexical_analyzer.h"

using namespace std;

int main() {

    string file_name = "D:\\college terms\\term 7\\Compiler\\compiler_project\\test.txt";
    string input_text = "if x == 10 {\n"
                        "    y = x + 5;\n"
                        "}";

    vector<DfaNode*> start_nodes = read_language_input(file_name);
    vector<LexemeClass> results = lexicalAnalyzer(input_text, start_nodes);

    for (LexemeClass item : results) {
        cout << item.lexemeClass << " " << item.lexeme << endl;
    }
    return 0;
}
