/*

#####################
#####################
#### INTERPRETER ####
#####################
#####################

*/

#include "interpreter.h"
#include "statuses.h"
#include "basicfunctions.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <random>
using namespace std;

Token no_token { Token(Value("", TokenType::SOL), 0) };

int nested_layer {0};
int target_layer {-1}; // NA FALSE CONDITIONS
int target_loop_layer {-1}; // NA WHILE LOOPS
unordered_set<int> true_conditions {-1}; // NA TRUE CONDITIONS
map<int, int> true_while_loops {}; // 1. int - layer, 2. int - index in lines variable

void makeup_numerals(vector<Token> &tokens, int &start_i, int &end_i, int &to_erase);
void make_operation(vector<Token> &tokens, vector<Token*> inter_tokens, int &i, double (*make_val)(double, double), int &end_i, int &to_erase);
void neg(vector<Token> &tokens, int &start_i, int &end_i, int &to_erase);
bool is_zero(string &num);
void cancel_comparision(vector<Token> &tokens, int &i, int &start_i, int &end_i, int &to_erase);
void print_array(string array_index);
void delete_array(string array_index);
string copy_array(string array_index);
bool is_temp_array(string array_index);
Value* get_variable_indexing(vector<Token>& tokens, int start_i, int &end_i, int &to_erase, bool delete_tokens);
void untemp_array(string array_index); // used only in ASSIGNER loop

inline double _power(double num1, double num2)
{
    return pow(num1, num2);
}

inline double _multiplier(double num1, double num2)
{
    return num1 * num2;
}

inline double _divisor(double num1, double num2)
{
    return num1 / num2;
}

inline double _plus(double num1, double num2)
{
    return num1 + num2;
}

inline double _minus(double num1, double num2)
{
    return num1 - num2;
}

inline double _modulo(double num1, double num2)
{
    return fmod(num1, num2);
}

inline bool return_boolean(Token& token)
{
    return ((token.value.t_type == TokenType::INTEGER || token.value.t_type == TokenType::FLOAT) && token.value.value.at(0) != '0') || (token.value.t_type == TokenType::BOOL && token.value.value == "TRUE") || (token.value.t_type != TokenType::BOOL && token.value.t_type != TokenType::INTEGER && token.value.t_type != TokenType::FLOAT && token.value.t_type != TokenType::NONETYPE);
}

int interpreter(vector<Token> &tokens, int start_i, int end_i, bool is_subprocess = false, bool is_looping = false)
{
    int to_erase = 0;
    const Token& first_token { get_token(tokens, 0) };

    if (!is_subprocess)
    {
        if (first_token.value.t_type == TokenType::KEYWORD)
        {
            if (first_token.value.value == "END")
            {
                nested_layer--;

                if (nested_layer < 0)
                    make_err(Status::ERROR, "Cannot use END without matching block");

                else if (nested_layer == target_loop_layer && true_while_loops.find(nested_layer) != true_while_loops.end())
                {
                    const int start_ln = true_while_loops.at(nested_layer);
                    const int end_ln = ln;
                    bool break_loop = false;
                    bool continue_loop = false;

                    const string condition = lines.at(start_ln-1);
                    vector<string> iterate_lines;

                    for (int i = start_ln; i < end_ln-1; i++)
                        iterate_lines.push_back(lines.at(i));

                    ln = start_ln;

                    while (true_while_loops.find(nested_layer) != true_while_loops.end())
                    {
                        target_loop_layer = -1;
                        nested_layer++;

                        for (const string line : iterate_lines)
                        {
                            ln++;
                            const vector<Token> line_tokens = lexer(line);
                            const string first_token_value = (line_tokens.size() != 0) ? line_tokens.at(0).value.value : "";

                            if (first_token_value == "BREAK")
                            {
                                if (target_layer == -1 && target_loop_layer == -1 && true_while_loops.size() != 0)
                                {
                                    break_loop = true;
                                    true_while_loops.erase(prev(true_while_loops.end())->first);

                                    if (true_conditions.find(nested_layer-1) != true_conditions.end())
                                        nested_layer--;

                                    break;
                                }
                            }

                            if (first_token_value == "CONTINUE")
                            {
                                if (target_layer == -1 && target_loop_layer == -1 && true_while_loops.size() != 0)
                                {
                                    continue_loop = true;

                                    if (true_conditions.find(nested_layer-1) != true_conditions.end())
                                        nested_layer--;

                                    break;
                                }

                                else
                                    continue;
                            }
                            
                            priemyslovak(line, false);
                        }

                        ln = start_ln;
                        nested_layer--;
                        
                        if (break_loop)
                        {
                            break_loop = false;
                            break;
                        }

                        else if (continue_loop)
                            continue_loop = false;

                        priemyslovak(condition, true);
                        nested_layer--;
                    }

                    ln = end_ln;
                }
            }

            else if (first_token.value.value == "IF" || first_token.value.value == "ELIF" || first_token.value.value == "ELSE" || first_token.value.value == "WHILE" || first_token.value.value == "FOR")
            {
                nested_layer++;
            }
        }

        if (target_layer != -1)
        {
            if (target_layer == nested_layer)
            {
                target_layer = -1;
            }
            else
                return 0;
        }

        if (target_loop_layer != -1)
        {
            if (target_loop_layer == nested_layer)
            {
                target_loop_layer = -1;
            }
            else
                return 0;
        }
    }

    // BRACKETS () - first priority
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };

        if (token.value.t_type == TokenType::L_BRACKET)
        {
            int start_i2 {i+1};
            int open_brackets {1};

            while (open_brackets != 0)
            {
                i++;
                Token& iter_token = tokens.at(i);

                if (iter_token.value.t_type == TokenType::L_BRACKET)
                    open_brackets++;

                else if (iter_token.value.t_type == TokenType::R_BRACKET)
                    open_brackets--;
            }

            int to_erase2 { interpreter(tokens, start_i2, i, true) };

            tokens.erase(tokens.begin() + start_i2-1);
            tokens.erase(tokens.begin() + start_i2);

            end_i -= 2 + to_erase2;
            to_erase += 2 + to_erase2;
            i -= 2 + to_erase2;
        }
    }

    // ARRAYS
    for (int i {start_i}; i < end_i; i++)
    {
        if (get_token(tokens, i).value.t_type == TokenType::SL_BRACKET)
        {
            vector<Token> array;
            const int start_bracket {i};
            int open_brackets {1};

            while (true)
            {
                i++;
                Token& iter_token { get_token(tokens, i) };

                if (iter_token.value.t_type == TokenType::SL_BRACKET)
                    open_brackets++;

                else if (iter_token.value.t_type == TokenType::SR_BRACKET)
                    open_brackets--;

                if (iter_token.value.t_type == TokenType::SEPARATOR && open_brackets == 1)
                {
                    const int to_erase_element { interpreter(tokens, start_bracket+1, i, true) };

                    array.push_back(tokens.at(start_bracket+1));
                    tokens.erase(tokens.begin() + start_bracket + 1, tokens.begin() + start_bracket + 3);

                    i = start_bracket;
                    end_i -= to_erase_element + 2;
                    to_erase += to_erase_element + 2;
                }

                else if (iter_token.value.t_type == TokenType::SR_BRACKET && open_brackets == 0)
                {
                    Token& to_add { tokens.at(start_bracket+1) };
                    string str_array_num;

                    if (to_add.value.t_type != TokenType::SR_BRACKET)
                    {
                        const int to_erase_element { interpreter(tokens, start_bracket+1, i, true) };

                        array.push_back(tokens.at(start_bracket+1));
                        tokens.erase(tokens.begin() + start_bracket, tokens.begin() + start_bracket + 3);

                        for (int j = 0 ;; j++)
                        {
                            str_array_num = to_string(j);
                            if (arrays.find(str_array_num) == arrays.end()) break;
                        }

                        arrays.insert_or_assign(str_array_num, array);
                        temp_arrays.push_back(str_array_num);
                        tokens.insert(tokens.begin() + start_bracket, Token(Value(str_array_num, TokenType::ARRAY), start_bracket));

                        end_i -= to_erase_element + 2;
                        to_erase += to_erase_element + 2;
                    }

                    else
                    {
                        tokens.erase(tokens.begin() + start_bracket, tokens.begin() + start_bracket + 2);

                        for (int j = 0 ;; j++)
                        {
                            str_array_num = to_string(j);
                            if (arrays.find(str_array_num) == arrays.end()) break;
                        }

                        arrays.insert_or_assign(str_array_num, array);
                        temp_arrays.push_back(str_array_num);
                        tokens.insert(tokens.begin() + start_bracket, Token(Value(str_array_num, TokenType::ARRAY), start_bracket));

                        end_i--;
                        to_erase++;
                    }

                    i = start_bracket;
                    break;
                }
            }
        }
    }

    // INPUT keyword
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };

        if (token.value.t_type == TokenType::KEYWORD && token.value.value == "INPUT")
        {
            string input;
            getline(cin, input);
            token.value = Value(input, TokenType::STRING);
        }
    }

    int assigner_pos {-1};

    // IDENTIFIERS
    for (int i {start_i}; i < end_i; i++)
    {
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& token { get_token(tokens, i) };
        Token& next_token { get_token_ranged(tokens, i+1, start_i, end_i) };

        for (int j {0}; j < end_i; j++) if (get_token(tokens, j).value.t_type == TokenType::ASSIGNER) assigner_pos = j;

        if (token.value.t_type == TokenType::IDENTIFIER && i+1 > assigner_pos && prev_token.value.value != "DELETE")
        {
            try
            {
                Value& var = identifiers.at(token.value.value);
                token = Token(Value(var.value, var.t_type), token.col);
            }
            catch (...)
            {
                make_err(Status::ERROR, "Variable \"" + token.value.value + "\" is undeclared", token.col);
            }
        }
    }

    // INDEXING
    for (int i {start_i}; i < end_i; i++)
    {
        Token& prev_token = get_token(tokens, i-1);
        Token& token { get_token(tokens, i) };

        for (int j {0}; j < end_i; j++) if (get_token(tokens, j).value.t_type == TokenType::ASSIGNER) assigner_pos = j;

        if (prev_token.value.value != "DELETE" && i+1 > assigner_pos && (token.value.t_type == TokenType::ARRAY || token.value.t_type == TokenType::IDENTIFIER))
        {
            Value* val = get_variable_indexing(tokens, i, end_i, to_erase, true);
            get_token(tokens, i).value = *val;
        }
    }

    // CONVERSION KEYWORDS
    for (int i = start_i; i < end_i; i++)
    {
        Token& token = get_token(tokens, i);
        Token& next_token = get_token(tokens, i+1);

        if (token.value.t_type == TokenType::KEYWORD)
        {
            if (token.value.value == "RANDOM")
            {
                random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<double> dist(0.0, 1.0);

                token.value = Value(to_string(dist(gen)), TokenType::FLOAT);
            }

            else if (token.value.value == "TO_STRING")
            {
                next_token.value.t_type = TokenType::STRING;
                tokens.erase(tokens.begin() + i, tokens.begin() + i+1);

                i--;
                end_i--;
                to_erase++;
            }

            else if (token.value.value == "TO_INT")
            {
                try
                {
                    const int val = round(stof(next_token.value.value));
                    next_token.value = Value(to_string(val), TokenType::INTEGER);
                    tokens.erase(tokens.begin() + i, tokens.begin() + i+1);

                    i--;
                    end_i--;
                    to_erase++;
                }
                catch(...)
                {
                    make_err(Status::ERROR, "Cannot convert this value to INTEGER");
                }
            }

            else if (token.value.value == "TO_FLOAT")
            {
                try
                {
                    const double val = stod(next_token.value.value);
                    next_token.value = Value(to_string(val), TokenType::FLOAT);
                    tokens.erase(tokens.begin() + i, tokens.begin() + i+1);

                    i--;
                    end_i--;
                    to_erase++;
                }
                catch(...)
                {
                    make_err(Status::ERROR, "Cannot convert this value to FLOAT");
                }
            }

            else if (token.value.value == "BREAK" || token.value.value == "CONTINUE")
                make_err(Status::ERROR, "Cannot use " + token.value.value + " outside of a loop");
        }
    }

    parser(tokens, start_i, end_i);
    makeup_numerals(tokens, start_i, end_i, to_erase);
    parser(tokens, start_i, end_i);

    // NEGATOR & POWER
    for (int i {end_i}; i >= start_i; i--)
    {
        makeup_numerals(tokens, start_i, end_i, to_erase);

        Token& token { get_token(tokens, i) };
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& prev_token2 { get_token_ranged(tokens, i-2, start_i, end_i) };

        if (prev_token.value.t_type == TokenType::POWER)
            make_operation(tokens, vector<Token*>{&prev_token2, &prev_token, &token}, i, &_power, end_i, to_erase);

        makeup_numerals(tokens, start_i, end_i, to_erase);
        parser(tokens, start_i, end_i);
    }

    // MULTIPLIER & DIVISOR & MODULO
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& prev_token2 { get_token_ranged(tokens, i-2, start_i, end_i) };

        if (prev_token.value.t_type == TokenType::MULTIPLIER || prev_token.value.t_type == TokenType::DIVISOR || prev_token.value.t_type == TokenType::MODULO)
        {
            if (prev_token.value.t_type == TokenType::DIVISOR && is_zero(token.value.value))
                make_err(Status::MATHERROR, "Cannot divide by zero", token.col);

            make_operation(tokens, vector<Token*>{&prev_token2, &prev_token, &token}, i, (prev_token.value.t_type == TokenType::MULTIPLIER) ? &_multiplier : (prev_token.value.t_type == TokenType::DIVISOR) ? &_divisor : &_modulo, end_i, to_erase);
        }
    }

    // PLUS & MINUS
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& prev_token2 { get_token_ranged(tokens, i-2, start_i, end_i) };

        if (prev_token.value.t_type == TokenType::PLUS)
        {
            if (prev_token2.value.t_type == TokenType::STRING && prev_token2.value.t_type == TokenType::STRING)
            {
                prev_token2.value.value += token.value.value;
                tokens.erase(tokens.begin() + i, tokens.begin() + i+2);
                i--;
                end_i -= 2;
                to_erase += 2;
            }
            
            else if (prev_token2.value.t_type == TokenType::STRING || prev_token2.value.t_type == TokenType::STRING)
                make_err(Status::SYNTAXERROR, "Invalid usage of " + prev_token2.value.t_type + " and " + token.value.t_type + " for addition");

            else
                make_operation(tokens, vector<Token*>{&prev_token2, &prev_token, &token}, i, &_plus, end_i, to_erase);
        }

        else if (prev_token.value.t_type == TokenType::MINUS)
            make_operation(tokens, vector<Token*>{&prev_token2, &prev_token, &token}, i, &_minus, end_i, to_erase);
        
    }

    parser(tokens, start_i, end_i);

    // COMPARATIONS
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& next_token { get_token_ranged(tokens, i+1, start_i, end_i) };
        Token& next_token2 { get_token_ranged(tokens, i+2, start_i, end_i) };

        if (token.value.t_type == TokenType::COMPARATOR)
        {
            bool eqeq = (prev_token.value.value == next_token.value.value && prev_token.value.t_type == next_token.value.t_type) || (is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count) && is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) && stod(prev_token.value.value) == stod(next_token.value.value));
            bool cancelled = false;

            //if (prev_token.value.t_type == TokenType::ERROR) continue;

            if (next_token2.value.t_type == TokenType::COMPARATOR)
            {
                //if (next_token.value.t_type == TokenType::ERROR)
                    //prev_token = Token(next_token.value.value, TokenType::ERROR, next_token.col);

                if (token.value.value == "==") { if (!eqeq) { cancel_comparision(tokens, i, start_i, end_i, to_erase); cancelled = true; }}

                else if (token.value.value == "~=") { if (eqeq) { cancel_comparision(tokens, i, start_i, end_i, to_erase); cancelled = true; }}

                else if (token.value.value == "<=") { if ((stod(prev_token.value.value) > stod(next_token.value.value))) { cancel_comparision(tokens, i, start_i, end_i, to_erase); cancelled = true; }}

                else if (token.value.value == ">=") { if ((stod(prev_token.value.value) < stod(next_token.value.value))) { cancel_comparision(tokens, i, start_i, end_i, to_erase); cancelled = true; }}

                else if (token.value.value == ">") { if ((stod(prev_token.value.value) <= stod(next_token.value.value))) { cancel_comparision(tokens, i, start_i, end_i, to_erase); cancelled = true; }}

                else { if ((stod(prev_token.value.value) >= stod(next_token.value.value))) { cancel_comparision(tokens, i, start_i, end_i, to_erase); cancelled = true; }}

                if (!cancelled)
                {
                    tokens.erase(tokens.begin() + i-1, tokens.begin() + i+1);
                    i--;
                    end_i -= 2;
                    to_erase += 2;
                }
            }

            else
            {
                //if (next_token.value.t_type == TokenType::ERROR)
                    //prev_token = Token(next_token.value.value, TokenType::ERROR, next_token.col);

                if (token.value.value == "==")
                    prev_token = Token(Value((eqeq) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);

                else if (token.value.value == "~=")
                    prev_token = Token(Value((!eqeq) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);

                else if (token.value.value == "<=")
                    prev_token = Token(Value((stod(prev_token.value.value) <= stod(next_token.value.value)) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);

                else if (token.value.value == ">=")
                    prev_token = Token(Value((stod(prev_token.value.value) >= stod(next_token.value.value)) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);

                else if (token.value.value == ">")
                    prev_token = Token(Value((stod(prev_token.value.value) > stod(next_token.value.value)) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);

                else
                    prev_token = Token(Value((stod(prev_token.value.value) < stod(next_token.value.value)) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);

                tokens.erase(tokens.begin() + i, tokens.begin() + i+2);
                end_i -= 2;
                to_erase += 2;
            }
        }
    }

    // COMPARATION KEYWORDS
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& next_token { get_token_ranged(tokens, i+1, start_i, end_i) };

        if (token.value.value == "OR")
        {
            bool val1 = (is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count) && !is_zero(prev_token.value.value)) || (prev_token.value.t_type == TokenType::BOOL && prev_token.value.value == "TRUE") || (prev_token.value.t_type != TokenType::BOOL && !is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count));
            
            if (!val1) if (next_token.value.t_type == TokenType::ERROR)
                make_err(Status::ERROR, next_token.value.value, next_token.col);
            
            bool val2 = (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) && !is_zero(next_token.value.value)) || (next_token.value.t_type == TokenType::BOOL && next_token.value.value == "TRUE") || (next_token.value.t_type != TokenType::BOOL && !is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count));

            prev_token = Token(Value((val1 || val2) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);
            tokens.erase(tokens.begin() + i, tokens.begin() + i+2);

            end_i -= 2;
            to_erase += 2;
            i -= 2;
        }

        else if (token.value.value == "AND")
        {
            if (prev_token.value.t_type == TokenType::ERROR)
                make_err(Status::ERROR, prev_token.value.value, prev_token.col);

            else if (next_token.value.t_type == TokenType::ERROR)
                make_err(Status::ERROR, next_token.value.value, next_token.col);

            bool val1 = (is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count) && !is_zero(prev_token.value.value)) || (prev_token.value.t_type == TokenType::BOOL && prev_token.value.value == "TRUE") || (prev_token.value.t_type != TokenType::BOOL && !is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count));
            bool val2 = (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) && !is_zero(next_token.value.value)) || (next_token.value.t_type == TokenType::BOOL && next_token.value.value == "TRUE") || (next_token.value.t_type != TokenType::BOOL && !is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count));

            prev_token = Token(Value((val1 && val2) ? "TRUE" : "FALSE", TokenType::BOOL), prev_token.col);
            tokens.erase(tokens.begin() + i, tokens.begin() + i+2);

            end_i -= 2;
            to_erase += 2;
            i -= 2;
        }
    }

    // KEYWORDS / ACTIONS
    for (int i {start_i}; i < end_i; i++)
    {
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& token { get_token(tokens, i) };
        Token& next_token { get_token_ranged(tokens, i+1, start_i, end_i) };
        Token& next_token2 { get_token_ranged(tokens, i+2, start_i, end_i) };

        if (token.value.t_type == TokenType::KEYWORD)
        {
            if (token.value.value == "PRINT" || token.value.value == "PRINTLN")
            {
                if (next_token.value.t_type == TokenType::ARRAY)
                    print_array(next_token.value.value);

                else
                {
                    if (token.value.value == "PRINTLN")
                        cout << next_token.value.value << "\n" << flush;

                    else
                        cout << next_token.value.value << flush;
                }
            }

            else if (token.value.value == "TERMINATE")
            {
                cout << "Terminating..." << "\n";
                exit(0);
            }

            else if (token.value.value == "SLEEP")
                this_thread::sleep_for(chrono::milliseconds(stoi(to_string(stod(next_token.value.value) * 1000))));

            else if (token.value.value == "DELETE")
            {
                try
                {
                    if (identifiers.at(next_token.value.value).t_type == TokenType::ARRAY && next_token2.value.t_type == TokenType::ARRAY)
                        //
                    
                    if (identifiers.at(next_token.value.value).t_type == TokenType::ARRAY)
                        delete_array(identifiers.at(next_token.value.value).value);
                    
                    identifiers.at(next_token.value.value);
                    identifiers.erase(next_token.value.value);
                }
                catch(...)
                {
                    make_err(Status::ERROR, "Variable \"" + next_token.value.value + "\" is undeclared", next_token.col);
                }
            }

            else if (token.value.value == "RAISE")
                make_err(Status::ERROR, next_token.value.value, token.col);
        
            else if (token.value.value == "GOTO")
            {
                const int line { stoi(next_token.value.value)-1 };
                priemyslovak(lines.at(line), false);
            }

            else if (token.value.value == "IMPORT")
                run_script(next_token.value.value);

            else if (token.value.value == "IF")
            {
                if (return_boolean(next_token))
                    true_conditions.insert(nested_layer-1);

                else
                {
                    target_layer = nested_layer-1;
                    true_conditions.erase(nested_layer-1);
                }
            }

            else if (token.value.value == "ELIF")
            {
                if (return_boolean(next_token) && true_conditions.find(nested_layer-1) == true_conditions.end())
                    true_conditions.insert(nested_layer-1);

                else
                    target_layer = nested_layer-1;
            }

            else if (token.value.value == "ELSE")
            {
                if (true_conditions.find(nested_layer-1) == true_conditions.end())
                    true_conditions.insert(nested_layer-1);

                else
                {
                    target_layer = nested_layer-1;
                    true_conditions.erase(nested_layer-1);
                }
            }

            else if (token.value.value == "WHILE")
            {
                target_loop_layer = nested_layer-1;

                if (return_boolean(next_token))
                    true_while_loops.insert_or_assign(target_loop_layer, ln);

                else
                    true_while_loops.erase(target_loop_layer);
            }

            else if (token.value.value == "FOR")
            {
                Token& next_token3 { get_token_ranged(tokens, i+3, start_i, end_i) }; // assign variable value
                Token& next_token5 { get_token_ranged(tokens, i+5, start_i, end_i) }; // after TO
                Token& next_token7 { get_token_ranged(tokens, i+7, start_i, end_i) }; // after STEP
                target_loop_layer = nested_layer-1;

                if (!is_looping) // beginning of looping
                {
                    identifiers.insert_or_assign(next_token.value.value, next_token3.value);
                    true_while_loops.insert_or_assign(target_loop_layer, ln);
                }

                else
                {
                    Value& val = identifiers.at(next_token.value.value);
                    const double double_val = stod(val.value);
                    const double step_val = (next_token7.value.t_type == TokenType::EOL) ? 1 : stod(next_token7.value.value);
                    const bool is_ints = double_val == static_cast<int>(round(double_val)) && step_val == static_cast<int>(round(step_val));
                    const double final = double_val + step_val;
                    const string str_final = to_string(final);
                    const string str_int_final = to_string(static_cast<int>(round(final)));

                    val = Value((is_ints) ? str_int_final : str_final, (is_ints) ? TokenType::INTEGER : TokenType::FLOAT);
                    const double num_val = stod(val.value);
                    const double num_to = stod(next_token5.value.value);
                    
                    if ((next_token7.value.value == "" && num_val <= num_to) || (next_token7.value.value != "" && ((next_token7.value.value.at(0) == '-' && num_val >= num_to) || (next_token7.value.value.at(0) != '-' && num_val <= num_to))))
                        true_while_loops.insert_or_assign(target_loop_layer, ln);
                    
                    else
                    {
                        true_while_loops.erase(target_loop_layer);
                        val.value = (is_ints) ? to_string(static_cast<int>(round(final-step_val))) : to_string(final-step_val);
                    }
                }

                break;
            }

            else if (token.value.value == "EXECUTE")
            {
                ln--;
                priemyslovak(next_token.value.value, false);
            }
        }
    }

    // VARIABLE VALUE ASSIGNING
    for (int i {start_i}; i < end_i; i++)
    {
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& token { get_token(tokens, i) };
        Token& next_token { get_token_ranged(tokens, i+1, start_i, end_i) };
        Token& next_token2 { get_token_ranged(tokens, i+2, start_i, end_i) };

        if (next_token.value.t_type == TokenType::ASSIGNER && prev_token.value.value != "FOR")
        {
            Value assign_value { Value(next_token2.value.value, next_token2.value.t_type) };
            Value* val {nullptr};

            if (next_token2.value.t_type == TokenType::ARRAY)
            {
                if (is_temp_array(next_token2.value.value))
                    assign_value.value = copy_array(next_token2.value.value);
                else
                    untemp_array(next_token2.value.value);
            }

            try
            {
                val = get_variable_indexing(tokens, (get_token(tokens, 0).value == Value("FOR", TokenType::KEYWORD)) ? 1 : 0, end_i, to_erase, false);

                if (val->t_type == TokenType::ARRAY)
                    delete_array(val->value);

                *val = assign_value;
            }
            catch(...)
            {
                identifiers.insert_or_assign(token.value.value, assign_value);
            }
        }
    }

    if (!is_subprocess)
    {
        /*
        cout << endl << "TEMP: ";
        for (string s : temp_arrays) cout << s << " ";

        for (string t : temp_arrays)
        {
            cout << "TEMP_ARRAY " << t << endl;
        }

        cout << endl;
        cout << "ARRAYS SIZE: " << arrays.size() << endl;

        for (auto& [key, value] : arrays)
        {
            cout << "ARRAY " << key << endl;
            for (Token t : arrays.at(key)) cout << t << endl;
            cout << endl;
        }
        */

        for (string i : temp_arrays)
            delete_array(i);

        temp_arrays.clear();
    }

    return to_erase;
}

bool is_zero(string &num)
{
    bool is_zero {true};

    for (int j = 0; j < num.length(); j++)
    {
        if (!(num.at(j) == '0' || num.at(j) == '.'))
        {
            is_zero = false;
            break;
        }
    }

    return is_zero;
}

bool is_next_pow(vector<Token> &tokens, int i, int &end_i)
{
    bool is_pow {false};

    for (int j {i}; j < end_i; j++)
    {
        Token& iter_token { tokens.at(j) };

        if (iter_token.value.t_type != TokenType::PLUS && iter_token.value.t_type != TokenType::MINUS && iter_token.value.t_type != TokenType::NEGATOR && !is_in_array<TokenType, const TokenType*>(iter_token.value.t_type, numerical, numerical_count))
        {
            is_pow = iter_token.value.t_type == TokenType::POWER;
            break;
        }
    }

    return is_pow;
}

void makeup_numerals(vector<Token> &tokens, int &start_i, int &end_i, int &to_erase)
{
    bool is_power = false;

    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };
        Token& next_token { get_token_ranged(tokens, i+1, start_i, end_i) };
        Token& prev_token { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& prev_token2 { get_token_ranged(tokens, i-2, start_i, end_i) };

        if ((prev_token.value.t_type == TokenType::PLUS || prev_token.value.t_type == TokenType::MINUS) && (token.value.t_type == TokenType::PLUS || token.value.t_type == TokenType::MINUS))
        {
            if (token.value.t_type == TokenType::PLUS) token = Token(Value(prev_token.value.value, prev_token.value.t_type), i);
            else if (token.value.t_type == TokenType::MINUS)
            {
                if (prev_token.value.t_type == TokenType::PLUS)
                    token = Token(Value("-", TokenType::MINUS), i);
                else
                    token = Token(Value("+", TokenType::PLUS), i);
            }

            i--;
            end_i--;
            to_erase++;
            tokens.erase(tokens.begin() + i);
        }

        else if (!is_in_array<TokenType, const TokenType*>(prev_token2.value.t_type, numerical, numerical_count) && (prev_token.value.t_type == TokenType::PLUS || prev_token.value.t_type == TokenType::MINUS) && is_in_array<TokenType, const TokenType*>(token.value.t_type, numerical, numerical_count) && next_token.value.t_type != TokenType::POWER)
        {
            bool is_minus { token.value.value.at(0) == '-' };
            string val = (is_zero(token.value.value)) ? token.value.value : "-" + token.value.value;

            if (prev_token.value.t_type == TokenType::PLUS)
                token = Token(Value(token.value.value, token.value.t_type), i);
            else
                token = Token(Value((is_minus) ? token.value.value.substr(1, token.value.value.length()) : val, token.value.t_type), i);
            
            tokens.erase(tokens.begin() + i-1);
            i--;
            end_i--;
            to_erase++;
        }

        neg(tokens, start_i, end_i, to_erase);
    }
}

void neg(vector<Token> &tokens, int &start_i, int &end_i, int &to_erase)
{
    for (int i {start_i}; i < end_i; i++)
    {
        Token& token { get_token(tokens, i) };
        Token& prev_token = { get_token_ranged(tokens, i-1, start_i, end_i) };
        Token& prev_token2 = { get_token_ranged(tokens, i-2, start_i, end_i) };

        if (token.value.t_type == TokenType::NEGATOR && !is_next_pow(tokens, i, end_i))
        {
            Token& next_token { get_token(tokens, i+1) };
            
            switch (next_token.value.t_type)
            {
                case TokenType::INTEGER:
                {
                    int val { ~stoi(next_token.value.value) };
                    string stringval { string(to_string(val)) };
                    
                    if (val < 0)
                    {
                        token = Token(Value(string("-"), TokenType::MINUS), token.col);
                        next_token = Token(Value(stringval.substr(1, stringval.length()), TokenType::INTEGER), token.col);
                    }
                        
                    else
                    {
                        token = Token(Value(stringval, TokenType::INTEGER), token.col);
                        tokens.erase(tokens.begin() + i+1);
                        end_i--;
                        to_erase++;
                    }
                    
                    break;
                }

                case TokenType::BOOL:
                    token = Token(Value(((next_token.value.value == "TRUE") ? "FALSE" : "TRUE"), TokenType::BOOL), token.col);
                    tokens.erase(tokens.begin() + i+1);
                    end_i--;
                    to_erase++;
                    break;

                case TokenType::NEGATOR:
                    tokens.erase(tokens.begin() + i, tokens.begin() + i+2);
                    end_i -= 2;
                    to_erase += 2;
                    break;
            }
        }
    }
}

void make_operation(vector<Token> &tokens, vector<Token*> inter_tokens, int &i, double (*make_val)(double, double), int &end_i, int &to_erase)
{
    double num1 { stod(inter_tokens[0]->value.value) };
    double num2 { stod(inter_tokens[2]->value.value) };
    
    double val { make_val(num1, num2) };

    bool is_int = val == static_cast<int>(round(val));

    if (is_int)
        *inter_tokens[0] = Token(Value(to_string(static_cast<int>(round(val))), TokenType::INTEGER), inter_tokens[0]->col);
    else
        *inter_tokens[0] = Token(Value(to_string(val), TokenType::FLOAT), inter_tokens[0]->col);

    tokens.erase(tokens.begin() + i-1, tokens.begin() + i+1);
    i--;
    end_i -= 2;
    to_erase += 2;
}

void cancel_comparision(vector<Token> &tokens, int &i, int &start_i, int &end_i, int &to_erase)
{
    for (; i < end_i;)
    {
        Token token = get_token_ranged(tokens, i, start_i, end_i);

        if (token.value.t_type == TokenType::COMPARATOR)
        {
            tokens.erase(tokens.begin() + i-1, tokens.begin() + i+1);
            end_i -= 2;
            to_erase += 2;
            
            if (get_token(tokens, i).value.t_type != TokenType::COMPARATOR)
            {
                tokens.erase(tokens.begin() + i, tokens.begin() + i);
                token = get_token(tokens, i-1);
                get_token(tokens, i-1) = Token(Value("FALSE", TokenType::BOOL), token.col);
                break;
            }
        }

        else
            break;

    }
}

void print_array(string array_index)
{
    vector<Token>& array { arrays.at(array_index) };
    const long long unsigned int arr_size { array.size() };

    cout << "[";
    for (int i {0}; i < arr_size; i++)
    {
        if (array[i].value.t_type == TokenType::ARRAY)
        {
            print_array(array[i].value.value);
            cout << ((i+1 != arr_size) ? ", " : "");
        }

        else
            cout << ((array[i].value.t_type == TokenType::STRING) ? "'" + array[i].value.value + "'" : array[i].value.value) << ((i+1 != arr_size) ? ", " : "");
    }
    cout << "]";
}

void delete_array(string array_index)
{
    vector<Token>& array { arrays.at(array_index) };
    const long long unsigned int arr_size { array.size() };

    for (int i {0}; i < arr_size; i++)
    {
        if (array.at(i).value.t_type == TokenType::ARRAY && arrays.find(array.at(i).value.value) != arrays.end())
            delete_array(array.at(i).value.value);
    }

    arrays.erase(array_index);
}

string copy_array(string array_index) // return - cloned array index
{
    vector<Token> array { arrays.at(array_index) };

    for (int i {0}; i < array.size(); i++)
    {
        if (array.at(i).value.t_type == TokenType::ARRAY)
            array.at(i).value.value = copy_array(array.at(i).value.value);
    }

    const string arr_size { to_string(arrays.size()) };
    arrays.insert_or_assign(arr_size, array);

    return arr_size;
}

bool is_temp_array(string array_index)
{
    bool found {false};
    
    for (const auto& [key, value] : identifiers)
    {
        if (value.value == array_index && value.t_type == TokenType::ARRAY)
        {
            found = true;
            break;
        }
    }
    
    if (!found) // if array is temp
    {
        for (Token &t : arrays.at(array_index))
        {
            if (t.value.t_type == TokenType::ARRAY)
            {
                found = is_temp_array(t.value.value);
            }
        }
    }
    
    return found;
}

Value* get_variable_indexing(vector<Token>& tokens, int start_i, int &end_i, int &to_erase, bool delete_tokens) // delete_tokens useful if you want to return a value of variable
{
    Value* val {nullptr};
    bool is_first {true};

    for (int j {start_i}; j < end_i; j++)
    {
        Token& iter_token { get_token(tokens, j) };
    
        if (iter_token.value.t_type != TokenType::ARRAY && iter_token.value.t_type != TokenType::IDENTIFIER)
            break;

        else if (iter_token.value.t_type == TokenType::IDENTIFIER)
        {
            val = &identifiers.at(iter_token.value.value);
            
            if (delete_tokens && !is_first)
            {
                tokens.erase(tokens.begin() + j, tokens.begin() + j+1);
                j--;
                end_i--;
                to_erase++;
            }

            is_first = false;
        }

        else if (iter_token.value.t_type == TokenType::ARRAY)
        {
            if (val == nullptr)
            {
                val = &iter_token.value;
                
                if (delete_tokens && !is_first)
                {
                    tokens.erase(tokens.begin() + j, tokens.begin() + j+1);
                    j--;
                    end_i--;
                    to_erase++;
                }

                is_first = false;
                continue;
            }

            if (val->t_type != TokenType::ARRAY)
                make_err(Status::SYNTAXERROR, "Cannot iterate over non-iterable type (" + val->t_type + ")", iter_token.col);

            if (arrays.at(iter_token.value.value).size() > 1)
                make_err(Status::SYNTAXERROR, "Element selector must contain only one element", iter_token.col);

            vector<Token>& array { arrays.at(val->value) };
            int array_index { stoi(arrays.at(iter_token.value.value).at(0).value.value) };

            if (array_index < 0 || array.size() <= array_index)
                make_err(Status::MEMORYERROR, "Array index out of range", iter_token.col);
                        
            val = &array.at(array_index).value;

            if (delete_tokens && !is_first)
            {
                tokens.erase(tokens.begin() + j, tokens.begin() + j+1);
                j--;
                end_i--;
                to_erase++;
            }

            is_first = false;
        }
    }

    return val;
}

void untemp_array(string array_index)
{
    for (int j {0}; j < temp_arrays.size(); j++)
    {
        if (temp_arrays.at(j) == array_index)
        {
            vector<Token>& array { arrays.at(array_index) };
            temp_arrays.erase(temp_arrays.begin() + j, temp_arrays.begin() + j+1);

            for (int k {0}; k < array.size(); k++)
            {
                if (array.at(k).value.t_type == TokenType::ARRAY)
                    untemp_array(array.at(k).value.value);
            }
        }
    }
}

void run_script(string path)
{
    const string filename = path;
    const string ext = string(".priem");
    string cmd;
    ifstream file(filename);
        
    if (filename.length() < ext.length() || filename.compare(filename.length() - ext.length(), ext.length(), ext) != 0)
    {
        cout << "ImportError: This file is not a PRIEMYSLOVAK script. Scripts must end with \".priem\"!" << endl << "Terminating..." << endl;
        exit(1);
    }

    if (!file.is_open())
    {
        cout << "ImportError: Failed to open the script." << endl << "Terminating..." << endl;
        exit(1);
    }

    while (getline(file, cmd))
    {
        lines.push_back(cmd);
        ln++;
        priemyslovak(cmd, false);
    }
        
    file.close();
}

void priemyslovak(string cmd, bool is_looping_interpreter)
{
    //if (cmd == "") return;

    // Break down the code
    vector<Token> tokens { lexer(cmd) };

    // Validate + execute
    parser(tokens, 0, tokens.size(), true);
    interpreter(tokens, 0, tokens.size(), false, is_looping_interpreter);
}
