#include "RulesParser.h"

#include <map>
#include <stack>
#include <stdexcept>
#include <utility>

#include "../dfa_conversion/dfa_convertor.h"
#include "string_to_relation_converter.h"

#include <queue>
#include <set>

map<string, Symbol*> symbol_map;
set<char> special_characters = {'\\', '=', ':', '-', '|', '+', '*', '(', ')'};

Symbol* Symbol::NO_SYMBOL = new Symbol(inf, "NO_SYMBOL");

class Word {
public:
    enum class Type { Operator, symbol, Char };

    virtual ~Word() = default;
    virtual Type getType() const = 0; // Pure virtual function
};

class OperatorWord : public Word {
public:
    char operator_char;

    OperatorWord(char operator_char) : operator_char(operator_char) {}

    Type getType() const override {
        return Type::Operator;
    }
};

class symbolWord : public Word {
public:
    Symbol* input_symbol;

    symbolWord(std::string input) {
        this->input_symbol = symbol_map[input];
    }

    Type getType() const override {
        return Type::symbol;
    }
};

class CharWord : public Word {
public:
    int input_char;

    CharWord(int input_char) : input_char(input_char) {}

    Type getType() const override {
        return Type::Char;
    }
};

bool is_left_parenthesis(const Word *word);
bool is_right_parenthesis(const Word *word);
int precedence(char operator_char);

vector<Word*> extract_words_from_string(string input);
vector<Word*> add_concatenation_between_words(vector<Word*> input);
queue<Word*> convert_infix_to_postfix(vector<Word*> input);
Relation* convert_postfix_to_relation(queue<Word*> input);

Relation* get_relation_from_infix(string &input, map<string, Symbol*> input_symbol_map) {
    symbol_map = move(input_symbol_map);

    vector<Word*> words_before_and = extract_words_from_string(input);
    vector<Word*> words_after_adding_and = add_concatenation_between_words(words_before_and);

    queue<Word*> postfix = convert_infix_to_postfix(words_after_adding_and);
    return convert_postfix_to_relation(postfix);
}

pair<bool, char> get_char_at_pos(string &s, int &pos) {
    if (s[pos] == ' ') return {false, ' '};
    if (s[pos] != '\\') {
        if (special_characters.count(s[pos])) return {false, s[pos]};
        return {true, s[pos++]};
    }
    if (pos == s.size() - 1) throw invalid_argument("no character after after \\");
    if (s[pos+1] == 'L') return {false, 'L'};
    // return {true, s[++pos]};
    char res = s[pos+1];
    pos += 2;
    return {true, res};
}

vector<Word*> extract_words_from_string(string input) {
    vector<Word*> words;
    int length = input.length();

    for (int i = 0; i < length; ++i) {
        char current_char = input[i];

        if (current_char == '\\') {
            // Handle escaped characters
            if (i + 1 < length) {
                char next_char = input[i + 1];
                if (next_char == 'L') {
                    // Lambda symbol \L
                    words.push_back(new CharWord(EPSLON)); // Assuming EPSLON represents Lambda
                    i++; // Skip next character
                } else {
                    // Any other escaped character
                    words.push_back(new CharWord(next_char));
                    i++; // Skip next character
                }
            } else {
                throw invalid_argument("Invalid escape sequence at end of string.");
            }
        } else if (string("-|+*()").find(current_char) != string::npos) {
            words.push_back(new OperatorWord(current_char));
        } else if (isspace(current_char)) {
            continue;
        } else {
            string word;
            while (i < length) {
                auto [valid_char, c] = get_char_at_pos(input, i);
                if (!valid_char) break;

                word += c;
            }
            --i; // Adjust for the last increment in the loop
            if (symbol_map.count(word)) {
                words.push_back(new symbolWord(word));
            } else {
                for (char c : word)
                    words.push_back(new CharWord(c));
            }
        }
    }

    return words;
}

bool check_word_to_place_and(Word* word, bool is_left) {
    if (word->getType() != Word::Type::Operator)
        return true;

    char c = is_left ? '(' : ')';

    OperatorWord *operator_word = static_cast<OperatorWord*>(word);
    if (operator_word->operator_char == c
        || operator_word->operator_char == '|'
        || operator_word->operator_char == '-'
        || (operator_word->operator_char == '*' && !is_left)
        || (operator_word->operator_char == '+' && !is_left)
    ) return false;

    return true;
}

vector<Word*> add_concatenation_between_words(vector<Word*> input) {
    vector<Word*> result;

    for (size_t i = 0; i < input.size(); ++i) {
        result.push_back(input[i]);

        if (i < input.size() - 1 && check_word_to_place_and(input[i], true) && check_word_to_place_and(input[i+1], false)) {
            result.push_back(new OperatorWord('.'));
        }
    }

    return result;
}

queue<Word*> convert_infix_to_postfix(vector<Word*> input) {
    stack<Word*> operator_stack; // To hold operators
    queue<Word*> postfix;        // To build the postfix result

    for (Word* word : input) {
        if (word->getType() == Word::Type::symbol || word->getType() == Word::Type::Char) {
            // Operand: directly add to postfix
            postfix.push(word);
        } else if (word->getType() == Word::Type::Operator && !is_right_parenthesis(word) && !is_left_parenthesis(word)) {
            auto operator_word = static_cast<OperatorWord*>(word);
            // Operator: pop higher or equal precedence operators to postfix
            while (!operator_stack.empty() && operator_stack.top()->getType() == Word::Type::Operator) {
                auto top_operator = static_cast<OperatorWord*>(operator_stack.top());
                if (precedence(top_operator->operator_char) >= precedence(operator_word->operator_char)) {
                    postfix.push(top_operator);
                    operator_stack.pop();
                } else {
                    break;
                }
            }
            operator_stack.push(operator_word);
        } else if (is_left_parenthesis(word)) {
            // Left parenthesis: push to operator stack
            operator_stack.push(word);
        } else if (is_right_parenthesis(word)) {
            // Right parenthesis: pop until left parenthesis is found
            while (!operator_stack.empty() && !is_left_parenthesis(operator_stack.top())) {
                postfix.push(operator_stack.top());
                operator_stack.pop();
            }
            if (!operator_stack.empty() && is_left_parenthesis(operator_stack.top())) {
                operator_stack.pop(); // Pop the left parenthesis
            } else {
                throw runtime_error("error in converting from infix to postfix");
            }
        }
    }

    // Pop any remaining operators to postfix
    while (!operator_stack.empty()) {
        if (operator_stack.top()->getType() == Word::Type::Operator) {
            postfix.push(operator_stack.top());
        } else {
            throw runtime_error("error in converting from infix to postfix");
        }
        operator_stack.pop();
    }

    return postfix;
}

Relation* convert_postfix_to_relation(queue<Word*> input) {
    stack<Relation*> relation_stack;

    while (!input.empty()) {
        Word* word = input.front();
        input.pop();

        if (word->getType() == Word::Type::symbol) {
            auto symbol_word = static_cast<symbolWord*>(word);
            // Create a base Relation from symbolWord
            Relation* symbol_relation = new symbolRelation(symbol_word->input_symbol);
            relation_stack.push(symbol_relation);
        } else if (word->getType() == Word::Type::Char) {
            auto char_word = static_cast<CharWord*>(word);
            Relation* char_relation = new CharRelation(char_word->input_char);
            relation_stack.push(char_relation);
        } else if (word->getType() == Word::Type::Operator) {
            auto operator_word = static_cast<OperatorWord*>(word);
            switch (operator_word->operator_char) {
                case '|': {
                    if (relation_stack.size() < 2) throw runtime_error("Invalid postfix expression");
                    Relation* r2 = relation_stack.top(); relation_stack.pop();
                    Relation* r1 = relation_stack.top(); relation_stack.pop();
                    relation_stack.push(new OrRelation(r1, r2));
                    break;
                }
                case '.': {
                    if (relation_stack.size() < 2) throw runtime_error("Invalid postfix expression");
                    Relation* r2 = relation_stack.top(); relation_stack.pop();
                    Relation* r1 = relation_stack.top(); relation_stack.pop();
                    relation_stack.push(new AndRelation(r1, r2));
                    break;
                }
                case '-': {
                    if (relation_stack.size() < 2) throw runtime_error("Invalid postfix expression");
                    auto* r2 = static_cast<CharRelation*>(relation_stack.top()); relation_stack.pop();
                    auto* r1 = static_cast<CharRelation*>(relation_stack.top()); relation_stack.pop();

                    relation_stack.push(new RangeRelation(r1->c, r2->c));
                    break;
                }
                case '*': {
                    if (relation_stack.empty()) throw runtime_error("Invalid postfix expression");
                    Relation* r = relation_stack.top(); relation_stack.pop();
                    relation_stack.push(new ClosureRelation(r, false)); // Non-positive closure
                    break;
                }
                case '+': {
                    if (relation_stack.empty()) throw runtime_error("Invalid postfix expression");
                    Relation* r = relation_stack.top(); relation_stack.pop();
                    relation_stack.push(new ClosureRelation(r, true)); // Positive closure
                    break;
                }
                default:
                    throw runtime_error("Unknown operator in postfix expression");
            }
        }
    }

    if (relation_stack.size() != 1) {
        throw runtime_error("Invalid postfix expression");
    }

    return relation_stack.top();
}

bool is_left_parenthesis(const Word *word)
{
    return word->getType() == Word::Type::Operator && static_cast<const OperatorWord*>(word)->operator_char == '(';
}

bool is_right_parenthesis(const Word *word)
{
    return word->getType() == Word::Type::Operator && static_cast<const OperatorWord*>(word)->operator_char == ')';
}

int precedence(char operator_char)
{
    switch (operator_char) {
        case '(' : case ')' : return 0;
        case '-' : return 4;
        case '*' : case '+': return 3;
        case '.' : return 2;
        case '|': return 1;
        default:
            throw runtime_error("Unknown operator");
    }
}
