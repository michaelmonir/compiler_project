#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;


#include <string>
#include <vector>
using namespace std;

void trimString(string &str);
void removeConsecutiveSpaces(string &str);
void removeFirstAndLastChar(string &str);
pair<string, string> splitToken(string str, char delimiter);
vector<string> split(const string& str, char delimiter);
vector<string> split(const string& str, const string& delimiter);
void replaceAll(string &str, const string& from, const string& to);

#endif // STRING_UTILS_H