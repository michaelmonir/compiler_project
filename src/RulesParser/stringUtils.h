#ifndef STRING_UTILS_H
#define STRING_UTILS_H

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

vector<std::string> split(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0, end;

    if (str.empty())
        return tokens;

    if (str.find(delimiter, start) == string::npos)
        return {str};

    while ((end = str.find(delimiter, start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

void replaceAll(string &str, const string& from, const string& to) {
    if (from.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

#endif // STRING_UTILS_H