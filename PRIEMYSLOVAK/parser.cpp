/*

################
################
#### PARSER ####
################
################

*/

#include "parser.h"
#include "basicfunctions.h"
#include "statuses.h"
using namespace std;

Token& get_token(vector<Token> &tokens, int i)
{
    static Token token { Token(Value("", TokenType::EOL), i) };
    return (i < tokens.size()) ? tokens[i] : token;
}

Token& get_token_ranged(vector<Token> &tokens, int token_pos, int &start_i, int &end_i)
{
    static Token sol { Token(Value("", TokenType::SOL), start_i) };
    static Token eol { Token(Value("", TokenType::EOL), end_i) };
    return (token_pos >= start_i && token_pos < end_i) ? tokens.at(token_pos) : ((token_pos < start_i) ? sol : eol);
}

Token& get_prev_type_token(vector<Token> &tokens, TokenType tt, int i)
{
    for (i--; i > 0; i--)
        if (tokens[i].value.t_type == tt)
            return tokens[i];

    static Token token { Token(Value("", TokenType::UNKNOWN), 0) };
    return token;
}

Token& get_next_type_token(vector<Token> &tokens, TokenType tt, int i, int step)
{
    for (i++; i < tokens.size(); i++) if (tokens[i].value.t_type == tt)
    {
        if (step == 0) return tokens[i];
        else step--;
    }
    static Token token { Token(Value("", TokenType::UNKNOWN), 0) };
    return token;
}

inline bool is_token_array(TokenType tt)
{
    return tt == TokenType::ARRAY; //|| tt == TokenType::SL_BRACKET || tt == TokenType::SR_BRACKET;
}

// return false if error
void parser(vector<Token> &tokens, int start_i, int end_i, bool is_starting)
{
    int open_brackets {0};
    int open_sq_brackets {0};
    int b_col {0};
    bool err {false};

    for (int i {0}; i < tokens.size(); i++)
    {
        const Token prev_token2 { (i-2 >= start_i) ? tokens[i-2] : Token(Value("", TokenType::SOL), i-2) };
        const Token prev_token { (i-1 >= 0) ? tokens[i-1] : Token(Value("", TokenType::SOL), i-1) };
        const Token token { tokens[i] };
        const Token next_token { get_token(tokens, i+1) };
        const Token next_token2 { get_token(tokens, i+2) };
        const bool is_prev_sol { prev_token.value.t_type == TokenType::SOL || is_in_array<string, const string*>(prev_token.value.value, starts_sol, starts_sol_count) };
        const bool is_prev_datatype { is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, datatypes, datatypes_count) || is_in_array<string, const string*>(prev_token.value.value, datatype_keywords, datatype_keywords_count) || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::ARRAY || prev_token.value.t_type == TokenType::R_BRACKET || prev_token.value.t_type == TokenType::SR_BRACKET || prev_token.value.t_type == TokenType::ERROR || (prev_token.value.t_type == TokenType::KEYWORD) };
        const bool is_next_datatype { is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count) || is_in_array<string, const string*>(next_token.value.value, datatype_keywords, datatype_keywords_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::ARRAY || next_token.value.t_type == TokenType::NEGATOR || next_token.value.t_type == TokenType::L_BRACKET || next_token.value.t_type == TokenType::SL_BRACKET || next_token.value.t_type == TokenType::ERROR || next_token.value.t_type == TokenType::PLUS || next_token.value.t_type == TokenType::MINUS || (next_token.value.t_type == TokenType::KEYWORD && next_token.value.value == "INPUT") };

        // EMPTY BRACKETS
            if (token.value.t_type == TokenType::L_BRACKET && next_token.value.t_type == TokenType::R_BRACKET)
                make_err(Status::SYNTAXERROR, "Cannot execute an empty brackets", token.col);

        // L_BRACKET
            else if (token.value.t_type == TokenType::L_BRACKET)
            {
                open_brackets++;
                b_col = i+1;
            }

            // R_BRACKET
            else if (token.value.t_type == TokenType::R_BRACKET)
            {
                if (open_brackets == 0)
                    make_err(Status::SYNTAXERROR, "Cannot close a non-existent bracket", token.col);

                open_brackets--;
                b_col = i+1;
            }

            // SL_BRACKET
            else if (token.value.t_type == TokenType::SL_BRACKET)
            {
                open_sq_brackets++;
                b_col = i+1;
            }

            // SR_BRACKET
            else if (token.value.t_type == TokenType::SR_BRACKET)
            {
                if (open_sq_brackets == 0)
                    make_err(Status::SYNTAXERROR, "Cannot close a non-existent square bracket", token.col);

                open_sq_brackets--;
                b_col = i+1;
            }

            // OPERATOR
            else if (is_in_array<TokenType, const TokenType*>(token.value.t_type, op_list_parser, op_count_parser))
            {
                // ~ (NEGATOR)
                if (token.value.t_type == TokenType::NEGATOR && !((is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, op_list_parser, op_count_parser) || is_prev_sol || prev_token.value.t_type == TokenType::L_BRACKET || prev_token.value.t_type == TokenType::SL_BRACKET || prev_token.value.t_type == TokenType::SEPARATOR || prev_token.value.t_type == TokenType::KEYWORD || prev_token.value.t_type == TokenType::COMPARATOR) && (is_next_datatype && next_token.value.t_type != TokenType::STRING && next_token.value.t_type != TokenType::NONETYPE && next_token.value.t_type != TokenType::FLOAT))) err = true;

                // = (ASSIGNER)
                else if (token.value.t_type == TokenType::ASSIGNER)
                {
                    if (!(is_next_datatype && ((get_token(tokens, 0).value.t_type == TokenType::IDENTIFIER) && (prev_token.value.t_type == TokenType::SR_BRACKET || prev_token.value.t_type == TokenType::ARRAY)) || prev_token.value.t_type == TokenType::IDENTIFIER))
                        err = true;
                }

                // , (SEPARATOR)
                else if (token.value.t_type == TokenType::SEPARATOR && !(is_prev_datatype && is_next_datatype))
                    make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);

                // Others
                else if ((token.value.t_type != TokenType::NEGATOR && token.value.t_type != TokenType::ASSIGNER && token.value.t_type != TokenType::SEPARATOR) && !((token.value.t_type == TokenType::PLUS && (is_in_array<string, const string*>(next_token.value.value, datatype_keywords, datatype_keywords_count) || next_token.value.t_type == TokenType::STRING || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::L_BRACKET || (is_token_array(next_token.value.t_type) && is_token_array(next_token2.value.t_type))) && (prev_token.value.t_type == TokenType::STRING || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::R_BRACKET || prev_token.value.t_type == TokenType::SR_BRACKET)) || (is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count) || (prev_token.value.t_type == TokenType::STRING && is_in_array<string, const string*>(prev_token2.value.value, datatype_keywords, datatype_keywords_count)) || is_in_array<string, const string*>(prev_token.value.value, datatype_keywords, datatype_keywords_count) || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::R_BRACKET || (is_token_array(prev_token.value.t_type) && is_token_array(prev_token2.value.t_type)) || ((token.value.t_type == TokenType::PLUS || token.value.t_type == TokenType::MINUS) && (prev_token.value.t_type == TokenType::L_BRACKET || (is_token_array(prev_token.value.t_type) && is_token_array(prev_token2.value.t_type)) || prev_token.value.t_type == TokenType::SEPARATOR || is_prev_sol || prev_token.value.t_type == TokenType::KEYWORD || prev_token.value.t_type == TokenType::COMPARATOR || is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, op_list_parser, op_count_parser)))) && (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) || is_in_array<string, const string*>(next_token.value.value, datatype_keywords, datatype_keywords_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::NEGATOR || next_token.value.t_type == TokenType::L_BRACKET || (is_token_array(next_token.value.t_type) && is_token_array(next_token2.value.t_type)) || next_token.value.t_type == TokenType::PLUS || next_token.value.t_type == TokenType::MINUS))) err = true;

                if (err)
                    make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);
            }
    }

    if (!is_starting)
    {
        for (int i {start_i}; i < end_i; i++)
        {
            const Token prev_token2 { (i-2 >= start_i) ? tokens[i-2] : Token(Value("", TokenType::SOL), i-2) };
            const Token prev_token { (i-1 >= start_i) ? tokens[i-1] : Token(Value("", TokenType::SOL), i-1) };
            const Token token { tokens[i] };
            const Token next_token { get_token_ranged(tokens, i+1, start_i, end_i) };
            const Token next_token2 { get_token_ranged(tokens, i+2, start_i, end_i) };
            const Token next_token3 { get_token_ranged(tokens, i+3, start_i, end_i) };
            const bool is_prev_sol { prev_token.value.t_type == TokenType::SOL || is_in_array<string, const string*>(prev_token.value.value, starts_sol, starts_sol_count) };
            const bool is_prev_datatype { is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, datatypes, datatypes_count) || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::ARRAY || prev_token.value.t_type == TokenType::R_BRACKET || prev_token.value.t_type == TokenType::SR_BRACKET || prev_token.value.t_type == TokenType::ERROR };
            const bool is_next_datatype { is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::ARRAY || next_token.value.t_type == TokenType::NEGATOR || next_token.value.t_type == TokenType::L_BRACKET || next_token.value.t_type == TokenType::SL_BRACKET || next_token.value.t_type == TokenType::ERROR || next_token.value.t_type == TokenType::PLUS || next_token.value.t_type == TokenType::MINUS };

            // DATATYPES WITH NO OPERATION
            if ((next_token.value.t_type == TokenType::L_BRACKET && is_in_array<TokenType, const TokenType*>(token.value.t_type, datatypes, datatypes_count)) || (token.value.t_type == TokenType::R_BRACKET && is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count)) || ((is_in_array<TokenType, const TokenType*>(token.value.t_type, datatypes, datatypes_count) || token.value.t_type == TokenType::IDENTIFIER) && (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count) || next_token.value.t_type == TokenType::IDENTIFIER)))
                make_err(Status::SYNTAXERROR, "Expected operator between " + token.value.t_type + " and " + next_token.value.t_type, token.col);

            // COMPARATOR
            else if (token.value.t_type == TokenType::COMPARATOR)
            {
                // == and ~=
                if ((token.value.value == "==" || token.value.value == "~=") && !(is_prev_datatype && is_next_datatype)) err = true;
            
                // Others
                else if (token.value.value != "==" && token.value.value != "~=" && !(is_prev_datatype && !is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, nonnumerical, nonnumerical_count) && is_next_datatype && !is_in_array<TokenType, const TokenType*>(next_token.value.t_type, nonnumerical, nonnumerical_count))) err = true;

                if (err)
                    make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);
            }

            // KEYWORD
            else if (token.value.t_type == TokenType::KEYWORD)
            {
                if (token.value.value == "PRINT" || token.value.value == "PRINTLN") { if (!(is_prev_sol && is_next_datatype)) err = true; }

                else if (token.value.value == "END") { if (!(is_prev_sol && next_token.value.t_type == TokenType::EOL)) err = true; }

                else if (token.value.value == "IF" || token.value.value == "ELIF") { if (!(is_prev_sol && is_next_datatype && get_next_type_token(tokens, TokenType::KEYWORD, i).value.value == "THEN")) err = true; }

                else if (token.value.value == "ELSE") { if (!is_prev_sol) err = true; }

                else if (token.value.value == "AND" || token.value.value == "OR") { if (!(is_prev_datatype && is_next_datatype)) err = true; }

                else if (token.value.value == "END") { if (next_token.value.t_type != TokenType::EOL) err = true; }

                else if (token.value.value == "GOTO") { if (!(is_prev_sol && (next_token.value.t_type == TokenType::INTEGER || next_token.value.t_type == TokenType::IDENTIFIER))) err = true; }

                else if (token.value.value == "THEN") { if (!(!is_prev_sol && next_token.value.t_type == TokenType::EOL && (tokens[0].value.value == "IF" || tokens[0].value.value == "ELSE" || tokens[0].value.value == "ELIF"))) err = true; }

                else if (token.value.value == "FOR") { if (!(is_prev_sol && next_token2.value.t_type == TokenType::ASSIGNER && get_next_type_token(tokens, TokenType::KEYWORD, i).value.value == "TO" && (get_next_type_token(tokens, TokenType::KEYWORD, i, 1).value.value == "DO" || (get_next_type_token(tokens, TokenType::KEYWORD, i, 1).value.value == "STEP" && get_next_type_token(tokens, TokenType::KEYWORD, i, 2).value.value == "DO")))) err = true; }

                else if (token.value.value == "TO" || token.value.value == "STEP") { if (!(!is_prev_sol && tokens[0].value.value == "FOR" && is_next_datatype && !is_in_array<TokenType, const TokenType*>(next_token.value.t_type, nonnumerical, nonnumerical_count))) err = true; }

                // else if (token.value.value == "NEXT") { if (!(is_next_datatype || next_token.value.t_type == TokenType::EOL)) err = true; }
                // pravdepodobne nepridám kvôli zbytočnej zložitosti

                else if (token.value.value == "WHILE") { if (!(is_prev_sol && is_next_datatype && get_next_type_token(tokens, TokenType::KEYWORD, i).value.value == "DO")) err = true; }

                else if (token.value.value == "BREAK") { if (!(is_prev_sol && next_token.value.t_type == TokenType::EOL)) err = true; }

                else if (token.value.value == "CONTINUE") { if (!(is_prev_sol && next_token.value.t_type == TokenType::EOL)) err = true; }

                else if (token.value.value == "DO") { if (!(!is_prev_sol && tokens[0].value.value == "FOR" || tokens[0].value.value == "WHILE")) err = true; }

                else if (token.value.value == "TRY") { if (!is_prev_sol) err = true; }

                else if (token.value.value == "EXCEPT") { if (!(is_prev_sol && (next_token.value.t_type == TokenType::EOL || (next_token.value.t_type == TokenType::IDENTIFIER && next_token2.value.t_type == TokenType::EOL)))) err = true; }

                else if (token.value.value == "RETURN") { if (!(is_prev_sol && is_next_datatype)) err = true; }

                else if (token.value.value == "SLEEP") { if (!(is_prev_sol && (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::L_BRACKET || next_token.value.t_type == TokenType::PLUS))) err = true; }

                else if (token.value.value == "RAISE") { if (!(is_prev_sol && is_next_datatype)) err = true; }

                else if (token.value.value == "IMPORT") { if (!(is_prev_sol && next_token.value.t_type == TokenType::STRING)) err = true; }

                else if (token.value.value == "DELETE") { if (!(is_prev_sol && next_token.value.t_type == TokenType::IDENTIFIER)) err = true; }

                else if (token.value.value == "EXECUTE") { if (!(next_token.value.t_type == TokenType::STRING || next_token.value.t_type == TokenType::IDENTIFIER)) err = true; }

                else if (token.value.value == "TERMINATE") { if (!(is_prev_sol && next_token.value.t_type == TokenType::EOL)) err = true; }

                else if (token.value.value == "TO_STRING" || token.value.value == "TO_INT" || token.value.value == "TO_FLOAT") { if (!(is_next_datatype)) err = true; }

                if (err)
                    make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);
            }
        }
    }
    /*
    for (int i {start_i}; i < end_i; i++)
    {

        const Token prev_token2 { (i-2 >= start_i) ? tokens[i-2] : Token(Value("", TokenType::SOL), i-2) };
        const Token prev_token { (i-1 >= start_i) ? tokens[i-1] : Token(Value("", TokenType::SOL), i-1) };
        const Token token { tokens[i] };
        const Token next_token { get_token_ranged(tokens, i+1, start_i, end_i) };
        const Token next_token2 { get_token_ranged(tokens, i+2, start_i, end_i) };
        const Token next_token3 { get_token_ranged(tokens, i+3, start_i, end_i) };
        const bool is_prev_sol { prev_token.value.t_type == TokenType::SOL || is_in_array<string, const string*>(prev_token.value.value, starts_sol, starts_sol_count) };
        const bool is_prev_datatype { is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, datatypes, datatypes_count) || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::ARRAY || prev_token.value.t_type == TokenType::R_BRACKET || prev_token.value.t_type == TokenType::SR_BRACKET || prev_token.value.t_type == TokenType::ERROR };
        const bool is_next_datatype { is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::ARRAY || next_token.value.t_type == TokenType::NEGATOR || next_token.value.t_type == TokenType::L_BRACKET || next_token.value.t_type == TokenType::SL_BRACKET || next_token.value.t_type == TokenType::ERROR || next_token.value.t_type == TokenType::PLUS || next_token.value.t_type == TokenType::MINUS };

        // EMPTY BRACKETS
        if (token.value.t_type == TokenType::L_BRACKET && next_token.value.t_type == TokenType::R_BRACKET)
            make_err(Status::SYNTAXERROR, "Cannot execute an empty brackets", token.col);

        // DATATYPES WITH NO OPERATION
        else if ((next_token.value.t_type == TokenType::L_BRACKET && is_in_array<TokenType, const TokenType*>(token.value.t_type, datatypes, datatypes_count)) || (token.value.t_type == TokenType::R_BRACKET && is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count)) || ((is_in_array<TokenType, const TokenType*>(token.value.t_type, datatypes, datatypes_count) || token.value.t_type == TokenType::IDENTIFIER) && (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, datatypes, datatypes_count) || next_token.value.t_type == TokenType::IDENTIFIER)))
            make_err(Status::SYNTAXERROR, "Expected operator between " + token.value.t_type + " and " + next_token.value.t_type, token.col);

        // L_BRACKET
        else if (token.value.t_type == TokenType::L_BRACKET)
        {
            open_brackets++;
            b_col = i+1;
        }

        // R_BRACKET
        else if (token.value.t_type == TokenType::R_BRACKET)
        {
            if (open_brackets == 0)
                make_err(Status::SYNTAXERROR, "Cannot close a non-existent bracket", token.col);

            open_brackets--;
            b_col = i+1;
        }

        // SL_BRACKET
        else if (token.value.t_type == TokenType::SL_BRACKET)
        {
            open_sq_brackets++;
            b_col = i+1;
        }

        // SR_BRACKET
        else if (token.value.t_type == TokenType::SR_BRACKET)
        {
            if (open_sq_brackets == 0)
                make_err(Status::SYNTAXERROR, "Cannot close a non-existent square bracket", token.col);

            open_sq_brackets--;
            b_col = i+1;
        }

        // OPERATOR
        else if (is_in_array<TokenType, const TokenType*>(token.value.t_type, op_list_parser, op_count_parser))
        {
            cout << prev_token2.value << " " << prev_token.value << " " << tokens[i] << " " << get_token(tokens, i+1) << " " << get_token(tokens, i+2) << endl;

            // ~ (NEGATOR)
            if (token.value.t_type == TokenType::NEGATOR && !((is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, op_list_parser, op_count_parser) || is_prev_sol || prev_token.value.t_type == TokenType::L_BRACKET || prev_token.value.t_type == TokenType::SL_BRACKET || prev_token.value.t_type == TokenType::SEPARATOR || prev_token.value.t_type == TokenType::KEYWORD || prev_token.value.t_type == TokenType::COMPARATOR) && (is_next_datatype && next_token.value.t_type != TokenType::STRING && next_token.value.t_type != TokenType::NONETYPE && next_token.value.t_type != TokenType::FLOAT))) err = true;

            // = (ASSIGNER)
            else if (token.value.t_type == TokenType::ASSIGNER)
            {
                if (is_next_datatype) continue;
                else if ((tokens[0].value.t_type == TokenType::IDENTIFIER) && (prev_token.value.t_type == TokenType::SR_BRACKET || prev_token.value.t_type == TokenType::ARRAY)) continue;
                else if (prev_token.value.t_type == TokenType::IDENTIFIER) continue;
                else err = true;
            }

            // , (SEPARATOR)
            else if (token.value.t_type == TokenType::SEPARATOR && !(is_prev_datatype && is_next_datatype)) err = true;

            // Others
            else if ((token.value.t_type != TokenType::NEGATOR && token.value.t_type != TokenType::ASSIGNER && token.value.t_type != TokenType::SEPARATOR) && !((token.value.t_type == TokenType::PLUS && (next_token.value.t_type == TokenType::STRING || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::L_BRACKET || (is_token_array(next_token.value.t_type) && is_token_array(next_token2.value.t_type))) && (prev_token.value.t_type == TokenType::STRING || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::R_BRACKET || prev_token.value.t_type == TokenType::SR_BRACKET)) || (is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, numerical, numerical_count) || prev_token.value.t_type == TokenType::IDENTIFIER || prev_token.value.t_type == TokenType::R_BRACKET || (is_token_array(prev_token.value.t_type) && is_token_array(prev_token2.value.t_type)) || ((token.value.t_type == TokenType::PLUS || token.value.t_type == TokenType::MINUS) && (prev_token.value.t_type == TokenType::L_BRACKET || (is_token_array(prev_token.value.t_type) && is_token_array(prev_token2.value.t_type)) || prev_token.value.t_type == TokenType::SEPARATOR || is_prev_sol || prev_token.value.t_type == TokenType::KEYWORD || prev_token.value.t_type == TokenType::COMPARATOR || is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, op_list_parser, op_count_parser)))) && (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::NEGATOR || next_token.value.t_type == TokenType::L_BRACKET || (is_token_array(next_token.value.t_type) && is_token_array(next_token2.value.t_type)) || next_token.value.t_type == TokenType::PLUS || next_token.value.t_type == TokenType::MINUS))) err = true;

            if (err)
                make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);
        }

        // COMPARATOR
        else if (token.value.t_type == TokenType::COMPARATOR)
        {
            // == and ~=
            if ((token.value.value == "==" || token.value.value == "~=") && !(is_prev_datatype && is_next_datatype)) err = true;
        
            // Others
            else if (token.value.value != "==" && token.value.value != "~=" && !(is_prev_datatype && !is_in_array<TokenType, const TokenType*>(prev_token.value.t_type, nonnumerical, nonnumerical_count) && is_next_datatype && !is_in_array<TokenType, const TokenType*>(next_token.value.t_type, nonnumerical, nonnumerical_count))) err = true;

            if (err)
                make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);
        }

        // KEYWORD
        else if (token.value.t_type == TokenType::KEYWORD)
        {
            // vytvoriť funkciu ako get_next_token ale prev a potom to implementovať všade namiesto tokens[0]
            // a aby tá funkcia hľadala iba v danej zátvorke (a ostatné preskakovala)

            if (token.value.value == "PRINT") { if (!(is_prev_sol && is_next_datatype)) err = true; }

            //else if (token.value.value == "VAR") { if (!(is_prev_sol && next_token2.t_type == TokenType::ASSIGNER)) err = true; }

            else if (token.value.value == "IF") { if (!(is_prev_sol && is_next_datatype && get_next_type_token(tokens, TokenType::KEYWORD, i).value.value == "THEN")) err = true; }

            else if (token.value.value == "ELSE") { if (!is_prev_sol) err = true; }

            else if (token.value.value == "AND" || token.value.value == "OR") { if (!(is_prev_datatype && is_next_datatype)) err = true; }

            else if (token.value.value == "END") { if (next_token.value.t_type != TokenType::EOL) err = true; }

            else if (token.value.value == "FUNC") { if (!(is_prev_sol && next_token.value.t_type == TokenType::IDENTIFIER && next_token2.value.t_type == TokenType::L_BRACKET && get_token(tokens, tokens.size()-1).value.t_type == TokenType::R_BRACKET)) err = true; }

            else if (token.value.value == "GOTO") { if (!(is_prev_sol && (next_token.value.t_type == TokenType::INTEGER || next_token.value.t_type == TokenType::IDENTIFIER))) err = true; }

            else if (token.value.value == "THEN") { if (!(!is_prev_sol && tokens[0].value.value == "IF" || tokens[0].value.value == "ELSE")) err = true; }

            else if (token.value.value == "FOR") { if (!(is_prev_sol && next_token2.value.t_type == TokenType::ASSIGNER && get_next_type_token(tokens, TokenType::KEYWORD, i).value.value == "TO" && (get_next_type_token(tokens, TokenType::KEYWORD, i, 1).value.value == "DO" || (get_next_type_token(tokens, TokenType::KEYWORD, i, 1).value.value == "STEP" && get_next_type_token(tokens, TokenType::KEYWORD, i, 2).value.value == "DO")))) err = true; }

            else if (token.value.value == "TO" || token.value.value == "STEP") { if (!(!is_prev_sol && tokens[0].value.value == "FOR" && is_next_datatype && !is_in_array<TokenType, const TokenType*>(next_token.value.t_type, nonnumerical, nonnumerical_count))) err = true; }

            else if (token.value.value == "NEXT") { if (!(is_next_datatype || next_token.value.t_type == TokenType::EOL)) err = true; }

            else if (token.value.value == "WHILE") { if (!(is_prev_sol && get_next_type_token(tokens, TokenType::KEYWORD, i).value.value == "DO")) err = true; }

            else if (token.value.value == "DO") { if (!(!is_prev_sol && tokens[0].value.value == "FOR" || tokens[0].value.value == "WHILE")) err = true; }

            else if (token.value.value == "TRY") { if (!is_prev_sol) err = true; }

            else if (token.value.value == "EXCEPT") { if (!(is_prev_sol && (next_token.value.t_type == TokenType::EOL || (next_token.value.t_type == TokenType::IDENTIFIER && next_token2.value.t_type == TokenType::EOL)))) err = true; }

            else if (token.value.value == "RETURN") { if (!(is_prev_sol && is_next_datatype)) err = true; }

            else if (token.value.value == "SLEEP") { if (!(is_prev_sol && (is_in_array<TokenType, const TokenType*>(next_token.value.t_type, numerical, numerical_count) || next_token.value.t_type == TokenType::IDENTIFIER || next_token.value.t_type == TokenType::L_BRACKET || next_token.value.t_type == TokenType::PLUS))) err = true; }

            else if (token.value.value == "RAISE") { if (!(is_prev_sol && is_next_datatype)) err = true; }

            else if (token.value.value == "IMPORT") { if (!(is_prev_sol && next_token.value.t_type == TokenType::STRING)) err = true; }

            else if (token.value.value == "CALL" || token.value.value == "DELETE") { if (!(is_prev_sol && next_token.value.t_type == TokenType::IDENTIFIER)) err = true; }

            else if (token.value.value == "TERMINATE") { if (!(is_prev_sol && next_token.value.t_type == TokenType::EOL)) err = true; }

            if (err)
                make_err(Status::SYNTAXERROR, "Invalid usage of " + token.value.t_type, token.col);
        }
    }
    */

    if (open_brackets != 0)
        make_err(Status::SYNTAXERROR, "Invalid usage of L_BRACKET", b_col);

    else if (open_sq_brackets != 0)
        make_err(Status::SYNTAXERROR, "Invalid usage of SL_BRACKET", b_col);
}