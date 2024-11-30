// //
// // Created by pc on 11/29/2024.
// //

#include "RulesParser.h"
#include <fstream>
#include <iostream>
#include <map>
#include "stringUtils.h"
#include "string_to_relation_converter.h"

map<string, Symbol*> definitions_map;
map<string, Symbol*> definitions_expressions_map;
vector<string> keywords;
vector<RegularExpression*> regularExpressions;
vector<RegularDefinition*> regularDefinitions;
vector<string> punctuation;

bool check_regular_definition_and_extract_token(string input, string &token);
bool check_regular_expression_and_extract_token(string input, string &token);
void parseKeywords(string token);
void parsePunctuation(string token);
TokenType getTokenType(string token);
string get_relation_string_from_line(string line, string symbol_name);

int parseRules(string filepath)
{
    int line_number = 0;

    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Error: could not open file" << endl;
        return -1;
    }

    string line;
    while(getline(file,line)) {
        line_number++;
        trimString(line);
        TokenType type = getTokenType(line);

        switch (type) {
            case KEYWORD:
                parseKeywords(line);
                break;
            case PUNCTUATION:
                parsePunctuation(line);
                break;
            case REGULAR_DEFINITION:
                string token = "";
                check_regular_definition_and_extract_token(line, token);

                definitions_expressions_map[token] = new Symbol()
                break;
            case REGULAR_EXPRESSION:

                // todo: need to handel errors
                break;
            case UNDEFINED:
                cout << "Error: Undefined token at line " << line_number << endl;
                return -1;
        }

        file.close();

        for (const string& keyword: keywords){
            RegularExpression regularExpression(keyword, keyword, KEYWORD);
            regularExpressions.push_back(regularExpression);
        }

        for (const string& punc: punctuation){
            RegularExpression regularExpression(punc, punc, PUNCTUATION);
            regularExpressions.push_back(regularExpression);
        }

        return 0;
    }

    return 0;
}

void first_iteration(string filepath)
{
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Error: could not open file" << endl;
    }

    string line;

    while(getline(file,line)) {
        trimString(line);
        TokenType type = getTokenType(line);

        switch (type) {
            case KEYWORD: {
                parseKeywords(line);
                break;
            }
            case PUNCTUATION: {
                parsePunctuation(line);
                break;
            }
            case REGULAR_DEFINITION: {
                string symbol_name;
                check_regular_definition_and_extract_token(line, symbol_name);

                Symbol *symbol = new Symbol(symbol_name);
                definitions_expressions_map[symbol_name] = symbol;
                definitions_map[symbol_name] = symbol;
                break;
            }
            case REGULAR_EXPRESSION: {
                string symbol_name;
                check_regular_expression_and_extract_token(line, symbol_name);

                Symbol *symbol = new Symbol(symbol_name);
                definitions_expressions_map[symbol_name] = symbol;
                break;
            }
            case UNDEFINED: {
                cout << "Error: Undefined token at line " << endl;
            }
        }
    }

    file.close();
}

void second_iteration(string filepath)
{
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Error: could not open file" << endl;
    }

    string line;

    int line_number = 0;

    while(getline(file,line)) {
        line_number++;
        trimString(line);
        TokenType type = getTokenType(line);

        switch (type) {
            case KEYWORD: {
                break;
            }
            case PUNCTUATION: {
                break;
            }
            case REGULAR_DEFINITION: {
                string symbol_name;
                check_regular_definition_and_extract_token(line, symbol_name);

                string relation_string = get_relation_string_from_line(line, symbol_name);
                Relation *relation = get_relation_from_infix(relation_string, definitions_map);

                break;
            }
            case REGULAR_EXPRESSION: {
                string symbol_name;
                check_regular_expression_and_extract_token(line, symbol_name);

                string relation_string = get_relation_string_from_line(line, symbol_name);
                Relation *relation = get_relation_from_infix(relation_string, definitions_expressions_map);

                RegularExpression *regular_expression = new RegularExpression(symbol_name, relation, line_number);
                regularExpressions.push_back(regular_expression);

                break;
            }
            case UNDEFINED: {
                cout << "Error: Undefined token at line " << endl;
            }
        }
    }

    file.close();
}

TokenType getTokenType(string token) {
    if (token.front() == '{' && token.back() == '}')
        return KEYWORD;
    if (token.front() == '[' && token.back() == ']')
        return PUNCTUATION;
    string temp = "";
    if (check_regular_definition_and_extract_token(token, temp))
        return REGULAR_DEFINITION;
    if (check_regular_expression_and_extract_token(token, temp))
        return REGULAR_EXPRESSION;
    return UNDEFINED;
}

bool check_regular_definition_and_extract_token(string input, string &token) {
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '=' && input[i-1] != '\\') return true;
        token += input[i];
    }
    return false;
}

bool check_regular_expression_and_extract_token(string input, string &token) {
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == ':' && input[i-1] != '\\') return true;
        token += input[i];
    }
    return false;
}

string get_string_after_parsing_special_characters(string input) {
    string result;

    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '\\') {
            if (i == input.size() - 1)
                throw invalid_argument("no character after \\");
            result += input[++i];
        }
    }
    return result;
}

void parseKeywords(string token) {
    removeFirstAndLastChar(token);
    trimString(token);
    removeConsecutiveSpaces(token);
    vector<string> keywordsList = split(token, ' ');
    for (const string& keyword: keywordsList)
        keywords.push_back(get_string_after_parsing_special_characters(keyword));
}

void parsePunctuation(string token) {
    removeFirstAndLastChar(token);
    trimString(token);
    removeConsecutiveSpaces(token);
    vector<string> punctuationList = split(token, ' ');
    for (const string& punc: punctuationList)
        punctuation.push_back(get_string_after_parsing_special_characters(punc));
}

string get_relation_string_from_line(string line, string symbol_name) {
    return line.substr(symbol_name.length());
}