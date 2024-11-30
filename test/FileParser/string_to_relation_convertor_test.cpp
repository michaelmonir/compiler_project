#include <gtest/gtest.h>

#include "../src/RulesParser/string_to_relation_converter.h"
#include <map>

using namespace std;

string get_string_from_relation(Relation *relation) {
    switch(relation->getType()) {
        case Relation::RelationType::Or: {
            OrRelation *or_relation = dynamic_cast<OrRelation*>(relation);
            if (or_relation) {
                return "(" + get_string_from_relation(or_relation->r1) + " | " + get_string_from_relation(or_relation->r2) + ")";
            }
            break;
        }

        case Relation::RelationType::And: {
            AndRelation *and_relation = dynamic_cast<AndRelation*>(relation);
            if (and_relation) {
                return "(" + get_string_from_relation(and_relation->r1) + " . " + get_string_from_relation(and_relation->r2) + ")";
            }
            break;
        }

        case Relation::RelationType::Closure: {
            ClosureRelation *closure_relation = dynamic_cast<ClosureRelation*>(relation);
            if (closure_relation) {
                string closure_symbol = closure_relation->is_positive ? "+" : "*";
                return "(" + get_string_from_relation(closure_relation->relation) + closure_symbol + ")";
            }
            break;
        }

        case Relation::RelationType::Token: {
            TokenRelation *token_relation = dynamic_cast<TokenRelation*>(relation);
            if (token_relation) {
                return "token{" + token_relation->token->token_name + "}";
            }
            break;
        }

        case Relation::RelationType::Char: {
            CharRelation *char_relation = dynamic_cast<CharRelation*>(relation);
            if (char_relation) {
                return "char{" + to_string(char_relation->c) + "}";
            }
            break;
        }

        case Relation::RelationType::Range: {
            RangeRelation *range_relation = dynamic_cast<RangeRelation*>(relation);
            if (range_relation) {
                return "range{" + to_string(range_relation->c1) + "-" + to_string(range_relation->c2) + "}";
            }
            break;
        }

        default:
            return "Unknown Relation";
    }
    return "Invalid Relation"; // In case of an invalid type
}

TEST(StringToRelationConvertor, test1)
{
    map<string, Token*> token_map;

    token_map["letter"] = new Token(1, "letter");
    token_map["digit"] = new Token(2, "digit");

    string input = "letter (letter|digit)*";

    Relation *relation = get_relation_from_infix(input, token_map);

    string output = get_string_from_relation(relation);

    cout << output << endl;
}