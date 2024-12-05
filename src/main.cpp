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
    string file_name = "/home/ryad/compiler_project/test.txt";
    string input_text = "int main() {\n"
                        "    float x = 3;\n"
                        "    int y = 42;\n"
                        "    boolean flag = true;\n"
                        "\n"
                        "    if (x >= y) {\n"
                        "        x = x + y * 2;\n"
                        "    } else {\n"
                        "        y = y - x / 3;\n"
                        "    }"
                        "    while (flag) {"
                        "        x = x - 1;"
                        "        if (x == 0 || x < 0) {\n"
                        "            flag = false;\n"
                        "        }\n"
                        "    }\n"
                        "}";
    // string input_text = "";
    vector<DfaNode*> start_nodes = read_language_input(file_name);
    vector<LexemeClass> results = lexicalAnalyzer(input_text, start_nodes);

    for (LexemeClass item : results) {
        cout << item.lexemeClass << " " << item.lexeme << endl;
    }
    return 0;
}
