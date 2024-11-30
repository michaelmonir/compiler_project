//
// Created by pc on 11/30/2024.
//

#ifndef COMPILER_PROJECT_STRINGUTILS_H
#define COMPILER_PROJECT_STRINGUTILS_H

#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

void trimString(string& str);
void removeConsecutiveSpaces(string& str);
void removeFirstAndLastChar(string& str);
pair<string, string> splitToken(string str, char delimiter);
vector<string> split(const string& str, char delimiter);
string replaceAll(string str, string toReplace, string replaceWith);


#endif //COMPILER_PROJECT_STRINGUTILS_H
