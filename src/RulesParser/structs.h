//
// Created by michael on 12/4/24.
//

#ifndef STRUCTS_H
#define STRUCTS_H

#define EPSLON 256
#define NFA_INPUT_SIZE 257
#define DFA_INPUT_SIZE 256
#define inf 100000000

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>

using namespace std;

struct LexemeClass {
    string lexemeClass;
    string lexeme;
};

class Symbol
{
public:
    string symbol_name;
    int index;
    static Symbol *NO_SYMBOL;

    Symbol(int symbol_index, string symbol_name) : index(symbol_index), symbol_name(symbol_name) {}
};

class Relation
{
public:
    enum class RelationType
    {
        Or,
        And,
        Closure,
        symbol,
        Char,
        Range,
        String
    };

    virtual ~Relation() = default;
    virtual RelationType getType() const = 0; // Pure virtual function
};

class Rule
{
public:
    Relation *relation;
    Symbol *symbol;
    int token_index;

    Rule(Symbol *symbol, Relation *rel, int token_index)
        : symbol(symbol), relation(rel), token_index(token_index) {}
};

class OrRelation : public Relation
{
public:
    Relation *r1, *r2;

    OrRelation(Relation *r1, Relation *r2) : r1(r1), r2(r2) {}

    RelationType getType() const override
    {
        return RelationType::Or;
    }
};

class AndRelation : public Relation
{
public:
    Relation *r1, *r2;

    AndRelation(Relation *r1, Relation *r2) : r1(r1), r2(r2) {}

    RelationType getType() const override
    {
        return RelationType::And;
    }
};

class ClosureRelation : public Relation
{
public:
    Relation *relation;
    bool is_positive;

    ClosureRelation(Relation *relation, bool is_positive) : relation(relation), is_positive(is_positive) {}

    RelationType getType() const override
    {
        return RelationType::Closure;
    }
};

class symbolRelation : public Relation
{
public:
    Symbol *symbol;

    symbolRelation(Symbol *symbol) : symbol(symbol) {}

    RelationType getType() const override
    {
        return RelationType::symbol;
    }
};

class CharRelation : public Relation
{
public:
    int c;

    CharRelation(int c) : c(c) {}

    RelationType getType() const override
    {
        return RelationType::Char;
    }
};

class RangeRelation : public Relation
{
public:
    int c1, c2;

    RangeRelation(int c1, int c2) : c1(c1), c2(c2) {}

    RelationType getType() const override
    {
        return RelationType::Range;
    }
};

class StringRelation : public Relation
{
public:
    string s;

    StringRelation(string s) : s(s) {}

    RelationType getType() const override
    {
        return RelationType::String;
    }
};

class Token {
public:
    int index;
    string token_name;

    static Token NO_TOKEN;

    Token(int index, string name) : index(index), token_name(name) {}
    Token() : index(-1), token_name("NO_TOKEN") {}  // Default constructor

    bool operator<(const Token& other) const {
        return index < other.index;
    }

    bool operator==(const Token& other) const {
        return token_name == other.token_name;
    }
    bool operator!=(const Token& other) const {
        return  token_name != other.token_name;
    }
};

class DfaNode {
public:
    static int dfa_nodes_counter;

    int dfa_node_index;
    map<int, DfaNode*> neighbors;
    Token token;

    DfaNode() : dfa_node_index(dfa_nodes_counter++), token(Token::NO_TOKEN) {}
};

class NfaNode {
public:
    static int nfa_nodes_counter;
    int nfa_node_index;
    vector<vector<NfaNode*>> neighbors;
    Token token;
    bool is_final;

    NfaNode()
        : nfa_node_index(nfa_nodes_counter++),
        neighbors(NFA_INPUT_SIZE),
        token(Token::NO_TOKEN) {}
    void print(unordered_set<int>& visited) const {
        // Avoid revisiting already printed nodes
        if (visited.count(nfa_node_index)) {
            cout << "NfaNode " << nfa_node_index << " (already printed)\n";
            return;
        }

        // Mark this node as visited
        visited.insert(nfa_node_index);

        // Print the node's details
        cout << "NfaNode {\n";
        cout << "  nfa_node_index: " << nfa_node_index << ",\n";
        cout << "  neighbors: {\n";

        for (size_t i = 0; i < neighbors.size(); ++i) {
            if (!neighbors[i].empty()) {
                cout << "    [" << i << "]: { ";
                for (const auto* neighbor : neighbors[i]) {
                    cout << neighbor->nfa_node_index << " ";
                }
                cout << "}\n";
            }
        }

        cout << "  },\n";
        cout << "  token: " << (token == Token::NO_TOKEN ? "NO_TOKEN" : "SOME_TOKEN") << ",\n";
        cout << "  is_final: " << (is_final ? "true" : "false") << "\n";
        cout << "}\n";

        // Recursively print neighbors
        for (const auto& neighborList : neighbors) {
            for (const auto* neighbor : neighborList) {
                neighbor->print(visited);
            }
        }
    }
};



#endif //STRUCTS_H
