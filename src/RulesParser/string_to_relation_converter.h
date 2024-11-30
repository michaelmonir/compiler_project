//
// Created by michael on 11/30/24.
//

#ifndef STRING_TO_RELATION_CONVERTER_H
#define STRING_TO_RELATION_CONVERTER_H

#include <map>

using namespace std;

class Symbol
{
public:
    string symbol_name;

    static struct symbol_s NO_symbol;

    Symbol(string symbol_name) : symbol_name(symbol_name) {}
};

class Relation {
public:
    enum class RelationType { Or, And, Closure, symbol, Char, Range };

    virtual ~Relation() = default;
    virtual RelationType getType() const = 0; // Pure virtual function
};

class OrRelation : public Relation {
public:
    Relation *r1, *r2;

    OrRelation(Relation *r1, Relation *r2) : r1(r1), r2(r2) {}

    RelationType getType() const override {
        return RelationType::Or;
    }
};

class AndRelation : public Relation {
public:
    Relation *r1, *r2;

    AndRelation(Relation *r1, Relation *r2) : r1(r1), r2(r2) {}

    RelationType getType() const override {
        return RelationType::And;
    }
};

class ClosureRelation : public Relation {
public:
    Relation *relation;
    bool is_positive;

    ClosureRelation(Relation *relation, bool is_positive) : relation(relation), is_positive(is_positive) {}

    RelationType getType() const override {
        return RelationType::Closure;
    }
};

class symbolRelation : public Relation {
public:
    Symbol *symbol;

    symbolRelation(Symbol *symbol) : symbol(symbol) {}

    RelationType getType() const override {
        return RelationType::symbol;
    }
};

class CharRelation : public Relation {
public:
    int c;

    CharRelation(int c) : c(c) {}

    RelationType getType() const override {
        return RelationType::Char;
    }
};

class RangeRelation : public Relation {
public:
    int c1, c2;

    RangeRelation(int c1, int c2) : c1(c1), c2(c2) {}

    RelationType getType() const override {
        return RelationType::Range;
    }
};

Relation* get_relation_from_infix(string &input, map<string, Symbol*> input_symbol_map);

#endif //STRING_TO_RELATION_CONVERTER_H
