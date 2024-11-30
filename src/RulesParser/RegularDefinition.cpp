//
// Created by pc on 11/30/2024.
//
#include "stringUtils.h"
#include "RegularDefinition.h"

int RegularDefinition::formalizeRegex(vector<RegularDefinition> definitions){
    removeConsecutiveSpaces(regex);
    addParentheses(regex);
    regex = parenthesizeDisjunctions(regex);
    expandRange(regex);
    expandDefinitions(regex, definitions);
    return 0;
}

void RegularDefinition::processAccumulator(vector<char> &stack, vector<char> &result,
                                           const unordered_map<string, string> &definitionMap) {
    if (!stack.empty()) {
        string accStr(stack.begin(), stack.end()); // Convert to string for lookup
        auto it = definitionMap.find(accStr);

        if (it != definitionMap.end()) {
            // Add the regex, wrapped in parentheses
            result.push_back('(');
            result.insert(result.end(), it->second.begin(), it->second.end());
            result.push_back(')');
        } else {
            // Add the raw accumulator
            result.insert(result.end(), stack.begin(), stack.end());
        }

        stack.clear(); // Clear the accumulator for reuse
    }
}

void RegularDefinition::expandDefinitions(string &regex, const vector<RegularDefinition> &regularDefinitions) {
    // Precompute a hash map for constant-time lookups
    unordered_map<string, string> definitionMap;
    for (const auto &def : regularDefinitions) {
        definitionMap[def.name] = def.regex;
    }

    vector<char> result;
    vector<char> stack;
    const string reservedChars = "=+*|()"; // Delimiter characters

    // Iterate through the input string character by character
    for (size_t i = 0; i < regex.length(); ++i) {
        char c = regex[i];
        bool isReservedChar = reservedChars.find(c) != string::npos;

        if (c == ' ' || (isReservedChar && (i == 0 || regex[i - 1] != '\\'))) {
            processAccumulator(stack, result, definitionMap); // Process accumulated characters
            result.push_back(c);                                   // Add delimiter directly
        } else {
           stack.push_back(c); // Accumulate the current character
        }
    }

    processAccumulator(stack, result, definitionMap); // Handle any remaining content in the accumulator

    // Convert result vector back to string
    regex = string(result.begin(), result.end());
}

string RegularDefinition::parenthesizeDisjunctions(string &str) {
    string result;
    string accumulator;
    bool disjunctionFound = false; // Tracks whether a disjunction is encountered
    const auto processDisjunction = [&](bool spaceBefore, bool spaceAfter) {
        result += "(" + accumulator + ")"; // Add the left operand
        result += spaceBefore ? " |" : "|"; // Handle space before `|`
        if (spaceAfter) result += " "; // Handle space after `|`
        accumulator.clear(); // Clear the accumulator for the next operand
        disjunctionFound = true; // Mark that a disjunction has been processed
    };

    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i];
        if (c == '|') { // Handle disjunction
            bool spaceBefore = (!accumulator.empty() && result.back() == ' ');
            bool spaceAfter = (i + 1 < str.length() && str[i + 1] == ' ');
            if (spaceBefore && !accumulator.empty()) accumulator.pop_back(); // Remove trailing space
            processDisjunction(spaceBefore, spaceAfter);
            if (spaceAfter) ++i; // Skip space after `|`
        } else if (c == '(') { // Handle nested parentheses
            int j = i + 1, depth = 1;
            while (j < str.length() && depth > 0) {
                if (str[j] == '(') ++depth;
                if (str[j] == ')') --depth;
                ++j;
            }
            string betweenBrackets = str.substr(i + 1, j - i - 1);
            accumulator += "(" + parenthesizeDisjunctions(betweenBrackets) + ")";
            i = j - 1; // Skip to the closing parenthesis
        } else { // Accumulate normal characters
            accumulator += c;
        }
    }

    // Finalize the result
    if (!accumulator.empty()) {
        result += disjunctionFound ? "(" + accumulator + ")" : accumulator;
    }

    return result;
}



void RegularDefinition:: addParentheses(string &regex) {
    static const char delimiters[] = {' ', '+', '*', '|', '('}; // Delimiters for finding closure boundaries

    for (size_t i = 1; i < regex.length(); ++i) {
        char c = regex[i];
        // Only process closures (+ or *) and ensure they are not escaped
        if ((c == '+' || c == '*') && regex[i - 1] != '\\') {
            if (regex[i - 1] != ')') {
                // Find the position to insert the opening parenthesis
                size_t j = i - 1;
                while (j > 0 && find(begin(delimiters), end(delimiters), regex[j]) == end(delimiters)) {
                    --j;
                }

                if (j == 0 && find(begin(delimiters), end(delimiters), regex[j]) == end(delimiters)) {
                    j--; // Handle edge case where term starts at the beginning
                }

                regex.insert(j + 1, "(");  // Add opening parenthesis
                regex.insert(i + 2, ")");  // Add closing parenthesis
                i += 2; // Adjust the index to skip over the newly inserted parentheses
            }
        }
    }
}

int RegularDefinition::expandRange(string &regex) {
    vector<char> expanded;
    for (int i = 0; i < regex.length(); ++i) {
        char currentChar = regex[i];
        if(currentChar != '-') {
            expanded.push_back(currentChar);
            continue;
        }
        if(regex[i-1] == '\\') {
            expanded.push_back(currentChar);
            continue;
        }
        // now the current regex is a range
        // Check if a valid alphanumeric range exists around the hyphen (with optional spaces).
        if ((isalnum(regex[i-1]) || isalnum(regex[i-2])) && (isalnum(regex[i+1]) || isalnum(regex[i+2]))) {
            // Remove extra space or character from result.
            int eraseIndex = (regex[i-1] != ' ') ? 1 : 2;
            expanded.erase(expanded.end() - eraseIndex, expanded.end());


            // Determine the range boundaries.
            char start = (regex[i-1] != ' ') ? regex[i-1] : regex[i-2];
            char end = (regex[i+1] != ' ') ? regex[i+1] : regex[i+2];

            // Expand and append the range.
            expanded.push_back('(');
            for (char ch = start; ch <= end; ++ch) {
                expanded.push_back('(');
                expanded.push_back(ch);
                expanded.push_back(')');

                if (ch != end) expanded.push_back('|');  // Add '|' separator between characters.
            }
            expanded.push_back(')');

            // Adjust the index to skip processed characters.
            i += (regex[i+1] != ' ') ? 1 : 2;
        } else {
            return -1;  // Return error if not a valid range.
        }
    }

    regex = string(expanded.begin(), expanded.end());
    return 0;
}
