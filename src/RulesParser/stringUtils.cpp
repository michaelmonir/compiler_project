//
// Created by pc on 11/29/2024.
//

#include <string>
#include <algorithm>

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
