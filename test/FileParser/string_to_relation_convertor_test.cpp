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

        case Relation::RelationType::symbol: {
            symbolRelation *symbol_relation = dynamic_cast<symbolRelation*>(relation);
            if (symbol_relation) {
                return "symbol{" + symbol_relation->symbol->symbol_name + "}";
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
    map<string, Symbol*> symbol_map;

    symbol_map["letter"] = new Symbol("letter");
    symbol_map["digit"] = new Symbol("digit");

    string input = "letter (letter|digit)*";

    Relation *relation = get_relation_from_infix(input, symbol_map);

    string output = get_string_from_relation(relation);

    EXPECT_EQ(output, "(symbol{letter} . ((symbol{letter} | symbol{digit})*))");
}

TEST(StringToRelationConvertor, test2)
{
    map<string, Symbol*> symbol_map;

    string input = "a-z | A-Z";

    Relation *relation = get_relation_from_infix(input, symbol_map);

    string output = get_string_from_relation(relation);

    EXPECT_EQ(output, "(range{97-122} | range{65-90})");
}

TEST(StringToRelationConvertor, test3)
{
    map<string, Symbol*> symbol_map;

    symbol_map["digit"] = new Symbol("digit");

    string input = "digit+";

    Relation *relation = get_relation_from_infix(input, symbol_map);

    string output = get_string_from_relation(relation);

    // cout << output << endl;
    EXPECT_EQ(output, "(symbol{digit}+)");
}

TEST(StringToRelationConvertor, test4)
{
    map<string, Symbol*> symbol_map;

    symbol_map["letter"] = new Symbol("letter");
    symbol_map["digit"] = new Symbol("digit");
    symbol_map["digits"] = new Symbol("digits");

    string input = " digit+ | digit+ . digits ( \\L | E digits)";

    Relation *relation = get_relation_from_infix(input, symbol_map);

    string output = get_string_from_relation(relation);

    EXPECT_EQ(output, "((symbol{digit}+) | ((((symbol{digit}+) . char{46}) . symbol{digits}) . (char{256} | (char{69} . symbol{digits}))))");
}
