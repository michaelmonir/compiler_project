//
// Created by michael on 11/30/24.
//

#ifndef STRING_TO_RELATION_CONVERTER_H
#define STRING_TO_RELATION_CONVERTER_H

#include <map>

using namespace std;

class Token
{
public:
    int index;
    string token_name;

    static struct token_s NO_TOKEN;

    Token(int index, string token_name) : index(index), token_name(token_name) {}

    bool operator<(const Token& other) const {
        return index < other.index;
    }

    bool operator==(const Token& other) const {
        return index == other.index && token_name == other.token_name;
    }
};

class Relation {
public:
    enum class RelationType { Or, And, Closure, Token, Char, Range };

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

class TokenRelation : public Relation {
public:
    Token *token;

    TokenRelation(Token *token) : token(token) {}

    RelationType getType() const override {
        return RelationType::Token;
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

Relation* get_relation_from_infix(string &input, map<string, Token*> input_token_map);

#endif //STRING_TO_RELATION_CONVERTER_H
