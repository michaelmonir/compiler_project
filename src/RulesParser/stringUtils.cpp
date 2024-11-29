//
// Created by pc on 11/29/2024.
//

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

void trimString(string &str) {
    str.erase(0, str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
}

void removeConsecutiveSpaces(string &str) {
    string::iterator new_end = unique(str.begin(), str.end(), [](char lhs, char rhs) {
        return (lhs == rhs) && (lhs == ' ');
    });
    str.erase(new_end, str.end());
}

void removeFirstAndLastChar(string &str) {
    if (str.size() < 2) return;
    str.erase(0, 1);
    str.erase(str.size() - 1, 1);
}

pair<string, string> splitToken(string str, char delimiter) {
    size_t pos = str.find(delimiter);
    if (pos == string::npos) return {str, ""};
    return {str.substr(0, pos), str.substr(pos + 1)};
}

vector<string> split(const string& str, char delimiter) {
    vector<std::string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}