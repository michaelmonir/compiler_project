// //
// // Created by pc on 11/29/2024.
// //

#include "RulesParser.h"
#include <fstream>
#include <iostream>
#include <map>
#include "stringUtils.h"
#include "string_to_relation_converter.h"

Token Token::NO_TOKEN = {inf, "NO TOKEN" };

map<string, Symbol*> definitions_map;
map<string, Symbol*> definitions_expressions_map;
vector<string> keywords;
vector<RegularExpression*> regularExpressions;
vector<RegularDefinition*> regularDefinitions;
vector<string> punctuations;

bool check_regular_expression_and_extract_token(string input, string &token, string &relation);
bool check_regular_definition_and_extract_token(string input, string &token, string &relation);
void parseKeywords(string token);
void parsePunctuation(string token);
TokenType getTokenType(string token);
string get_relation_string_from_line(string &line, string &symbol_name);

void first_iteration(string filepath);
void second_iteration(string filepath);

vector<Rule> parseRules(string filepath)
{
    first_iteration(filepath);
    second_iteration(filepath);

    for (string punctuation : punctuations) {
        RegularExpression *regular_expression
            = new RegularExpression(punctuation, new StringRelation(punctuation), PUNCTUATION);
        regularExpressions.push_back(regular_expression);
    }

    for (string keyword : keywords) {
        RegularExpression *regular_expression
            = new RegularExpression(keyword, new StringRelation(keyword), KEYWORD);
        regularExpressions.push_back(regular_expression);
    }

    sort(regularExpressions.begin(), regularExpressions.end(), [](RegularExpression *a, RegularExpression *b) {
        return a->priority < b->priority;
    });

    vector<Rule> rules;
    int x =0;
    for (int i = 0; i < regularExpressions.size(); i++) {
        RegularExpression *expression = regularExpressions[i];
        Rule *rule = new Rule(new Symbol(x, expression->symbol_name), expression->relation, i+1);
        x++;
        rules.push_back(*rule);
    }

    for (int i = 0; i < regularDefinitions.size(); i++) {
        RegularDefinition *definition = regularDefinitions[i];
        Rule *rule = new Rule(new Symbol(x, definition->symbol_name), definition->relation, inf);
        x++;
        rules.push_back(*rule);
    }

    return rules;
}

void first_iteration(string filepath)
{
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Error: could not open file" << endl;
    }

    string line;
    int line_number = 0;

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
                string symbol_name, relation_string;
                check_regular_definition_and_extract_token(line, symbol_name, relation_string);

                Symbol *symbol = new Symbol(line_number, symbol_name);
                definitions_expressions_map[symbol_name] = symbol;
                definitions_map[symbol_name] = symbol;
                break;
            }
            case REGULAR_EXPRESSION: {
                string symbol_name, relation_string;
                check_regular_expression_and_extract_token(line, symbol_name, relation_string);

                Symbol *symbol = new Symbol(line_number, symbol_name);
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

    while(getline(file,line)) {
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
                string symbol_name, relation_string;
                check_regular_definition_and_extract_token(line, symbol_name, relation_string);

                Relation *relation = get_relation_from_infix(relation_string, definitions_map);

                RegularDefinition *regular_definition = new RegularDefinition(symbol_name, relation);
                regularDefinitions.push_back(regular_definition);

                break;
            }
            case REGULAR_EXPRESSION: {
                string symbol_name, relation_string;
                check_regular_expression_and_extract_token(line, symbol_name, relation_string);

                Relation *relation = get_relation_from_infix(relation_string, definitions_expressions_map);

                RegularExpression *regular_expression = new RegularExpression(symbol_name, relation, REGULAR_EXPRESSION);
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
    string temp11, temp12, temp21, temp22;
    if (check_regular_definition_and_extract_token(token, temp11, temp12))
        return REGULAR_DEFINITION;
    if (check_regular_expression_and_extract_token(token, temp21, temp22))
        return REGULAR_EXPRESSION;
    return UNDEFINED;
}

bool check_regular_definition_and_extract_token(string input, string &token, string &relation) {
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '=' && input[i-1] != '\\') {
            relation = get_relation_string_from_line(input, token);
            trimString(token);
            return true;
        }
        token += input[i];
    }
    return false;
}

bool check_regular_expression_and_extract_token(string input, string &token, string &relation) {
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == ':' && input[i-1] != '\\') {
            relation = get_relation_string_from_line(input, token);
            trimString(token);
            return true;
        }
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
        else {
            result += input[i];
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
        punctuations.push_back(get_string_after_parsing_special_characters(punc));
}

string get_relation_string_from_line(string &line, string &symbol_name) {
    return line.substr(symbol_name.length() + 1);
}