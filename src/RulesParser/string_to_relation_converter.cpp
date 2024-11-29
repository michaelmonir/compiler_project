//
// Created by pc on 11/29/2024.
//
#include "RulesParser.h"

#include <assert.h>
#include <map>
#include <stack>
#include <stdexcept>

#include "../dfa_conversion/dfa_convertor.h"

class Token
{
public:
    int index;
    string token_name;

    static struct token_s NO_TOKEN;

    bool operator<(const Token& other) const {
        return index < other.index;
    }

    bool operator==(const Token& other) const {
        return index == other.index && token_name == other.token_name;
    }
};

map<string, Token*> token_map;

class Relation
{
};

class Word {
};

class OperatorWord : public Word {
public:
    char operator_char;

    OperatorWord(char operator_char) : operator_char(operator_char) {}
};

class TokenWord : public Word {
public:
    Token* input_token;

    TokenWord(string input) {
        this->input_token = token_map[input];
    }
};

class RelationWord : public Word {
public:
    Relation* relation;

    RelationWord(Relation* relation) : relation(relation) {}
};

class CharWord : public Word {
public:
    int input_char;

    CharWord(int input_char) : input_char(input_char) {}
};

vector<Word> extract_words_from_string(string input)
{
    vector<Word> words;
    size_t length = input.length();
    for (size_t i = 0; i < length; ++i)
    {
        char current_char = input[i];

        if (current_char == '\\')
        {
            // Handle escaped characters
            if (i + 1 < length)
            {
                char next_char = input[i + 1];
                if (next_char == 'L')
                {
                    // Lambda symbol \L
                    words.emplace_back(CharWord(EPSLON)); // Assuming '\0' represents Lambda
                    i++; // Skip next character
                }
                else
                {
                    // Any other escaped character
                    words.emplace_back(CharWord(next_char));
                    i++; // Skip next character
                }
            }
            else
            {
                throw invalid_argument("Invalid escape sequence at end of string.");
            }
        }
        else if (ispunct(current_char))
        {
            // Reserved symbols for regular expressions
            if (string("-|+*()").find(current_char) != string::npos)
            {
                words.emplace_back(OperatorWord(current_char));
            }
            else
            {
                throw invalid_argument("Unexpected punctuation character in input: " + string(1, current_char));
            }
        }
        else if (isalnum(current_char))
        {
            // Token word (could span multiple characters)
            string token_name;
            while (i < length && isalnum(input[i]))
            {
                token_name += input[i++];
            }
            --i; // Adjust for the last increment in the loop
            if (token_map.find(token_name) != token_map.end())
            {
                words.emplace_back(TokenWord(token_name));
            }
            else
            {
                // throw invalid_argument("Undefined token encountered: " + token_name);
            }
        }
        else if (isspace(current_char))
        {
            // Ignore whitespace
            continue;
        }
        else
        {
            assert(false);
        }
    }
    return words;
}

stack<Word> convert_infix_to_postfix(vector<Word> input) {
    stack<Word> postfix;
    return postfix;
}

Relation* convert_postix_to_relation(stack<Word> input) {
    // last word that comes from the stack is casted to relation word
    // then we return the relation inside this relation word
    return NULL;
}