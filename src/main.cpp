#include <iostream>

#include "RulesParser/rulesParser.h"

using namespace std;

int main() {
    RulesParser rulesParser("C:\\Users\\pc\\OneDrive-AlexandriaUniversity\\Documents\\GitHub\\compiler_project\\src\\test.txt");
    rulesParser.parseRules();
    for (RegularExpression re : rulesParser.getRegularExpressions()) {
        cout << re.toString()<<endl;
    }

    return 0;
}
