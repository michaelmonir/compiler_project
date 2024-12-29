//
// Created by pc on 12/28/2024.
//

#include "GrammarParser.h"
#include "../RulesParser/stringUtils.h"
#include <fstream>
#include <iostream>

set<ParseUnit*> nonTerminals;
set<ParseUnit*> terminals;
vector<ParseRule*> rules;

int check_rules(string rule){
    vector<string> parts = split(rule, "::=");
    if (parts.size() != 2) {
        cout << "Error: Invalid rule format" << endl;
        return -1;
    }
    trimString(parts[0]);
    trimString(parts[1]);
    if (parts[0].empty() || parts[1].empty()) {
        cout << "Error: Invalid rule format" << endl;
        return -1;
    }
    ParseUnit* nonTerminal = new ParseUnit();
    nonTerminal->name = parts[0];
    nonTerminal->type = ParseUnitType::NON_TERMINAL;

    if (nonTerminals.find(nonTerminal) != nonTerminals.end()){
        cout << "Error: Duplicate non-terminal" << endl;
        return -1;
    }
    nonTerminals.insert(nonTerminal);
    return 0;
}

int parseTerminals(string rhs){
    string terminal = "";
    for (int i = 0; i < rhs.length(); i++) {
       char c = rhs[i];
       // extract terminal
       if (c == '\'' and (i == 0 or rhs[i-1] != '\\')){
           rhs.replace(i, 1, " ");
           i++;

           while(rhs[i] != '\'' || rhs[i-1] == '\\' ){
               if(i == rhs.length()){
                   cout << "Error: no closing single quote" << endl;
                   return -1;
               }
               if(rhs[i] == '\\' and rhs[i+1] != 'L'){
                  i++;
                   continue;
               }
               terminal += rhs[i];
               i++;
           }
          rhs.replace(i, 1, " ");
       }
        if (!terminal.empty()){
            ParseUnit* terminalUnit = new ParseUnit();
            terminalUnit->name = terminal;
            terminalUnit->type = ParseUnitType::TERMINAL;
            terminals.insert(terminalUnit);
            terminal.clear();
        }
    }
    return 0;
}

int parseRules(string nonTerminal, string rhs){
    ParseRule* rule = new ParseRule();
    rule->lhs = nonTerminal;
    replaceAll(rhs, "\\|", "\\@");
    vector<string> ors = split(rhs, '|');

    for (string or_expr : ors){
        replaceAll(or_expr, "\\@", "\\|");
        trimString(or_expr);
        removeConsecutiveSpaces(or_expr);

        vector<string> ands = split(or_expr, ' ');
        vector<ParseUnit> and_expressions;
        for(string symbol : ands){
            if(symbol == "\\L") {
                continue;
            }
            if(symbol[0] == '\\'){
                symbol.erase(0, 1);
            }
            ParseUnit* unit = new ParseUnit();
            unit->name = symbol;
            unit->type = ParseUnitType::TERMINAL;
            if (terminals.find(unit) != terminals.end()){
                and_expressions.push_back(*unit);
            }else{
                unit->type = ParseUnitType::NON_TERMINAL;
                if (nonTerminals.find(unit) != nonTerminals.end()){
                    and_expressions.push_back(*unit);
                }else{
                    cout << "Error: Undefined terminal or non-terminal" << endl;
                    return -1;
                }
            }
        }
        rule->or_expressions.push_back(and_expressions);
    }
    rules.push_back(rule);
    return 0;
}

void GrammarParser::parseGrammar(const string filePath){
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error: File not found" << endl;
        return;
    }
    string line;
    while(getline(file,line, '#')) {
        trimString(line);
        if (line.empty()) {
            continue;
        }
        int check = check_rules(line);
        if (check == -1) {
            cout << "Error: Invalid rule" << endl;
            return;
        }
    }
    file.close();

    file.open(filePath);
    if (!file.is_open()) {
        cout << "Error: File not found" << endl;
        return;
    }
    while (getline(file, line, '#')) {
        trimString(line);
        if (line.empty()) {
            continue;
        }
        vector<string> parts = split(line, "::=");
        string lhs = parts[0];
        string rhs = parts[1];
        trimString(lhs);
        removeConsecutiveSpaces(lhs);
        trimString(rhs);
        removeConsecutiveSpaces(rhs);

        int check = parseTerminals(rhs);
        if (check == -1) {
            cout << "Error: Invalid terminal" << endl;
            return;
        }
        check = parseRules(lhs, rhs);
        if (check == -1) {
            cout << "Error: Invalid rule" << endl;
            return;
        }
    }
    file.close();
}

set<ParseUnit*> GrammarParser::getNonTerminals(){
    return nonTerminals;
}

set<ParseUnit*> GrammarParser::getTerminals(){
    return terminals;
}

vector<ParseRule*> GrammarParser::getRules(){
    return rules;
}
