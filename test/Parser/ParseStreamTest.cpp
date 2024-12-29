#include <gtest/gtest.h>
#include <map>
#include <stack>
#include <iostream>

using namespace std;

#include "Parser/ParcerStream.h"

// Global variables
map<ParseUnit, Token> _terminal_map;
map<ParseUnit, map<Token, ParseTableItem>> _parse_table;

// Helper functions for creating ParseUnits and Tokens
ParseUnit make_terminal(const string& name) {
    return {name, ParseUnitType::TERMINAL};
}

ParseUnit make_non_terminal(const string& name) {
    return {name, ParseUnitType::NON_TERMINAL};
}

Token make_token(int index, const string& name) {
    return Token(index, name);
}

void initialize_parse_table() {
    // Define terminals
    ParseUnit id = make_terminal("id");
    ParseUnit semicolon = make_terminal(";");
    ParseUnit assign = make_terminal("=");
    ParseUnit num = make_terminal("num");
    ParseUnit relop = make_terminal("relop");
    ParseUnit lparen = make_terminal("(");
    ParseUnit rparen = make_terminal(")");
    ParseUnit lbrace = make_terminal("{");
    ParseUnit rbrace = make_terminal("}");
    ParseUnit if_keyword = make_terminal("if");
    ParseUnit else_keyword = make_terminal("else");
    ParseUnit while_keyword = make_terminal("while");
    ParseUnit int_keyword = make_terminal("int");
    ParseUnit float_keyword = make_terminal("float");
    ParseUnit plus = make_terminal("float");
    ParseUnit minus = make_terminal("float");

    // Define non-terminals
    ParseUnit method_body = make_non_terminal("METHOD_BODY");
    ParseUnit statement_list = make_non_terminal("STATEMENT_LIST");
    ParseUnit statement = make_non_terminal("STATEMENT");
    ParseUnit declaration = make_non_terminal("DECLARATION");
    ParseUnit primitive_type = make_non_terminal("PRIMITIVE_TYPE");
    ParseUnit if_stmt = make_non_terminal("IF");
    ParseUnit while_stmt = make_non_terminal("WHILE");
    ParseUnit assignment = make_non_terminal("ASSIGNMENT");
    ParseUnit expression = make_non_terminal("EXPRESSION");
    ParseUnit simple_expression = make_non_terminal("SIMPLE_EXPRESSION");
    ParseUnit term = make_non_terminal("TERM");
    ParseUnit factor = make_non_terminal("FACTOR");
    ParseUnit sign = make_non_terminal("SIGN");

    // Add terminal map entries
    _terminal_map[id] = make_token(1, "id");
    _terminal_map[semicolon] = make_token(2, ";");
    _terminal_map[assign] = make_token(3, "=");
    _terminal_map[num] = make_token(4, "num");
    _terminal_map[relop] = make_token(5, "relop");
    _terminal_map[lparen] = make_token(6, "(");
    _terminal_map[rparen] = make_token(7, ")");
    _terminal_map[lbrace] = make_token(8, "{");
    _terminal_map[rbrace] = make_token(9, "}");
    _terminal_map[if_keyword] = make_token(10, "if");
    _terminal_map[else_keyword] = make_token(11, "else");
    _terminal_map[while_keyword] = make_token(12, "while");
    _terminal_map[int_keyword] = make_token(13, "int");
    _terminal_map[float_keyword] = make_token(14, "float");
    _terminal_map[plus] = make_token(15, "+");
    _terminal_map[minus] = make_token(16, "-");

    // Define parse table entries
    _parse_table[method_body][_terminal_map[int_keyword]] = {false, {statement_list}};
    _parse_table[method_body][_terminal_map[float_keyword]] = {false, {statement_list}};
    _parse_table[method_body][_terminal_map[if_keyword]] = {false, {statement_list}};
    _parse_table[method_body][_terminal_map[while_keyword]] = {false, {statement_list}};
    _parse_table[method_body][_terminal_map[id]] = {false, {statement_list}};

    _parse_table[statement_list][_terminal_map[int_keyword]] = {false, {statement, statement_list}};
    _parse_table[statement_list][_terminal_map[float_keyword]] = {false, {statement, statement_list}};
    _parse_table[statement_list][_terminal_map[if_keyword]] = {false, {statement, statement_list}};
    _parse_table[statement_list][_terminal_map[while_keyword]] = {false, {statement, statement_list}};
    _parse_table[statement_list][_terminal_map[id]] = {false, {statement, statement_list}};
    _parse_table[statement_list][_terminal_map[semicolon]] = {true, {}}; // FOLLOW(STATEMENT_LIST)

    _parse_table[statement][_terminal_map[int_keyword]] = {false, {declaration}};
    _parse_table[statement][_terminal_map[float_keyword]] = {false, {declaration}};
    _parse_table[statement][_terminal_map[if_keyword]] = {false, {if_stmt}};
    _parse_table[statement][_terminal_map[while_keyword]] = {false, {while_stmt}};
    _parse_table[statement][_terminal_map[id]] = {false, {assignment}};

    _parse_table[declaration][_terminal_map[int_keyword]] = {false, {primitive_type, id, semicolon}};
    _parse_table[declaration][_terminal_map[float_keyword]] = {false, {primitive_type, id, semicolon}};

    _parse_table[primitive_type][_terminal_map[int_keyword]] = {false, {int_keyword}};
    _parse_table[primitive_type][_terminal_map[float_keyword]] = {false, {float_keyword}};

    _parse_table[if_stmt][_terminal_map[if_keyword]] = {false, {if_keyword, lparen, expression, rparen, lbrace, statement, rbrace, else_keyword, lbrace, statement, rbrace}};

    _parse_table[while_stmt][_terminal_map[while_keyword]] = {false, {while_keyword, lparen, expression, rparen, lbrace, statement, rbrace}};

    _parse_table[assignment][_terminal_map[id]] = {false, {id, assign, expression, semicolon}};

    _parse_table[expression][_terminal_map[id]] = {false, {simple_expression}};
    _parse_table[expression][_terminal_map[num]] = {false, {simple_expression}};
    _parse_table[expression][_terminal_map[lparen]] = {false, {simple_expression}};
    _parse_table[expression][_terminal_map[relop]] = {false, {simple_expression, relop, simple_expression}};

    _parse_table[simple_expression][_terminal_map[id]] = {false, {term}};
    _parse_table[simple_expression][_terminal_map[num]] = {false, {term}};
    _parse_table[simple_expression][_terminal_map[lparen]] = {false, {term}};
    _parse_table[simple_expression][_terminal_map[sign]] = {false, {sign, term}};

    _parse_table[term][_terminal_map[id]] = {false, {factor}};
    _parse_table[term][_terminal_map[num]] = {false, {factor}};
    _parse_table[term][_terminal_map[lparen]] = {false, {factor}};

    _parse_table[factor][_terminal_map[id]] = {false, {id}};
    _parse_table[factor][_terminal_map[num]] = {false, {num}};
    _parse_table[factor][_terminal_map[lparen]] = {false, {lparen, expression, rparen}};

    _parse_table[sign][_terminal_map[plus]] = {false, {make_terminal("+")}};
    _parse_table[sign][_terminal_map[minus]] = {false, {make_terminal("-")}};
}


TEST(ParseStreamTest, ParseStreamTest1) {
    // Initialize parse table and stack
    initialize_parse_table();
    initialize_terminal_map_and_parse_table(_terminal_map, _parse_table);
    initialize_stack(make_non_terminal("METHOD_BODY"));

    vector<Token> input_tokens = {
        make_token(13, "int"), make_token(1, "x"), make_token(2, ";"),
        make_token(1, "x"), make_token(3, "="), make_token(4, "5"), make_token(2, ";"),
        make_token(10, "if"), make_token(6, "("), make_token(1, "x"), make_token(5, ">"), make_token(4, "2"), make_token(7, ")"),
        make_token(8, "{"), make_token(1, "x"), make_token(3, "="), make_token(4, "0"), make_token(2, ";"), make_token(9, "}")
    };

    // Process tokens
    for (const Token& token : input_tokens) {
        stream_process_input_token(token);
    }

    // Print the left-most derivation
    cout << "Left-most derivation: ";
    left_most_print_root();
    cout << endl;
}