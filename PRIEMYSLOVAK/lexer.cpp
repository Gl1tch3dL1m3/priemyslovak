/*

###############
###############
#### LEXER ####
###############
###############

*/

#include "lexer.h"
#include "statuses.h"
#include "basicfunctions.h"
using namespace std;

Token::Token(Value value, unsigned col)
: value(value), col(col)
{}

Value::Value(string value, TokenType t_type)
: value(value), t_type(t_type)
{}

Value::Value()
: value(""), t_type(TokenType::NONETYPE)
{}

bool is_comment {false};

vector<Token> lexer(string code)
{
    vector<Token> tokens;

    for (int col {0}; col < code.length(); col++)
    {
        const string commentcheck {string(1, code[col]) + string(1, code[col+1])};

        // Skip spaces
        if (code[col] == ' ' || code[col] == '	') continue;

        // Check break or one-line COMMENT
        else if (commentcheck == "//") break;

        // LONG COMMENT END
        if (commentcheck == "*/")
        {
            if (!is_comment) make_err(Status::SYNTAXERROR, "Cannot close a non-existent comment", col+1);
            
            is_comment = false;
            col += 2;

            // Skip spaces again
            if (code[col] == ' ' || code[col] == '	') continue;
        }

        // LONG COMMENT START
        else if (commentcheck == "/*" && !is_comment)
        {
            while (col < code.length() && !(code[col-2] == '*' && code[col-1] == '/')) col++;
            is_comment = !(code[col-2] == '*' && code[col-1] == '/');

            // Skip spaces again
            if (code[col] == ' ' || code[col] == '	') continue;
        }

        // Check COMMENT again
        if (is_comment) continue;

        const char& ch { code[col] };
        const string ch_str { string(1, ch) };
        const int start {col};

        if (ch == '\0') break;

        // INT, FLOAT
        if (isdigit(ch) || (ch == '.' && isdigit(code[col+1])))
        {
            while (col < code.length() && (isdigit(code[col]) || code[col] == '.' || code[col] == '_')) col++;
            string value { code.substr(start, col - start) };
            int dot_count { count_string('.', value) };

            if (dot_count > 1) make_err(Status::MATHERROR, "Float cannot contain more than one decimal point", col+1);
            else
            {
                string value_final {""};
                for (char& val_ch : value) if (isalnum(val_ch) || val_ch == '.') value_final += string(1, val_ch);
                if (value_final[value_final.size()-1] == '.') value_final += '0';
                else if (value_final[0] == '.') value_final = '0' + value_final;

                tokens.push_back(Token(Value(value_final, (dot_count == 0) ? TokenType::INTEGER : TokenType::FLOAT), start+1));
                col--;
            }
            
            continue;
        }
        
        // STR
        else if (ch == '"' || ch == '\'')
        {
            do 
            {
                col++;
                if (code[col] == '\\')// && (code[col+1] == ch || code[col+1] == '\\'))
                {
                    char &next_char = code[col+1];

                    switch (next_char)
                    {
                        case 'n':
                            next_char = '\n';
                            break;

                        case 't':
                            next_char = '\t';
                            break;

                        case 'r':
                            next_char = '\r';
                            break;

                        case 'b':
                            next_char = '\b';
                            break;
                    }

                    code.erase(col, 1);
                    continue;
                }

                if (code[col] == ch)
                    break;
            }
            while (col < code.length());
            string value { code.substr(start+1, col-start-1) };

            if (code[col] != ch) make_err(Status::SYNTAXERROR, "Unterminated string literal found", col+1);
            else tokens.push_back(Token(Value(value, TokenType::STRING), start+1));

            /*
            cout << "FIRST " << code[col+1] << endl;
            while (col < code.length() && code[col+1] != ch)
            {
                cout << code[col+1] << endl;
                if (code[col+1] != '\\') col++;
                else { code.erase(col+1, 1); };
                col++;
            }
            cout << code[col] << endl;
            col++;
            string value = code.substr(start+1, col-1-start);

            if (code[col] != ch) make_err(Status::SYNTAXERROR, "Unterminated string literal found", col+1);
            else tokens.push_back(Token(value, TokenType::STRING, start+1));
            */

            continue;
        }

        else if (isalpha(ch) || ch == '_')
        {
            // NONETYPE
            if (col + 3 < code.length() && upper(code.substr(col, 4)) == "NULL" && !isalnum(code[col+4]))
            {
                tokens.push_back(Token(Value("NULL", TokenType::NONETYPE), start+1));
                col += 3;
                continue;
            }

            // BOOL - TRUE
            else if (col + 3 < code.length() && upper(code.substr(col, 4)) == "TRUE" && !isalnum(code[col+4]))
            {
                tokens.push_back(Token(Value("TRUE", TokenType::BOOL), start+1));
                col += 3;
                continue;
            }

            // BOOL - FALSE
            else if (col + 4 < code.length() && upper(code.substr(col, 5)) == "FALSE" && !isalnum(code[col+5]))
            {
                tokens.push_back(Token(Value("FALSE", TokenType::BOOL), start+1));
                col += 4;
                continue;
            }

            // KEYWORD / IDENTIFIER
            else
            {
                // IDENTIFIER
                while (col < code.length() && (isalnum(code[col]) || code[col] == '_')) col++;
                string value { code.substr(start, col - start) };
                string up_value { upper(value) };

                // KEYWORD CHECK
                if (is_in_array<string, const string*>(up_value, keywords, keyword_count))
                    tokens.push_back(Token(Value(up_value, TokenType::KEYWORD), start+1));
                else
                    tokens.push_back(Token(Value(value, TokenType::IDENTIFIER), start+1));

                col--;
                continue;
            }
        }

        // OPERATOR
        else if (is_in_array<char, const char*>(ch, op_list, op_count))
        {
            // COMPARATOR
            if ((ch == '~' || ch == '=' || ch == '>' || ch == '<') && code[col+1] == '=')
            {
                tokens.push_back(Token(Value(string(1, ch) + string(1, code[col+1]), TokenType::COMPARATOR), start+1));
                col++;
            }

            // SQUARE BRACKETS
            else if (ch == '[')
                tokens.push_back(Token(Value(ch_str, TokenType::SL_BRACKET), start+1));

            else if (ch == ']')
                tokens.push_back(Token(Value(ch_str, TokenType::SR_BRACKET), start+1));

            else tokens.push_back(Token(Value(ch_str, (ch == '>' || ch == '<') ? TokenType::COMPARATOR :
                (ch == '(') ? TokenType::L_BRACKET:
                (ch == ')') ? TokenType::R_BRACKET:
                (ch == '+') ? TokenType::PLUS:
                (ch == '-') ? TokenType::MINUS:
                (ch == '*') ? TokenType::MULTIPLIER:
                (ch == '/') ? TokenType::DIVISOR:
                (ch == '^') ? TokenType::POWER:
                (ch == '%') ? TokenType::MODULO:
                (ch == '=') ? TokenType::ASSIGNER:
                (ch == ',') ? TokenType::SEPARATOR:
                TokenType::NEGATOR
            ), start+1));

            continue;
        }

        // ERROR
        make_err(Status::ERROR, "Unexpected character: '" + string(1, ch) + string(1, '\''), col+1);
    }

    return tokens;
}

ostream& operator<<(ostream& o, Token& token)
{
    o << "Token(\"" << token.value.value << "\", " << token.value.t_type << ", " << token.col << ")";
    return o;
}

string tokentype_to_string(TokenType tt)
{
    switch (tt)
    {
        case TokenType::INTEGER:
            return "INTEGER";
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::STRING:
            return "STRING";
        case TokenType::BOOL:
            return "BOOL";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::MULTIPLIER:
            return "MULTIPLIER";
        case TokenType::DIVISOR:
            return "DIVISOR";
        case TokenType::POWER:
            return "POWER";
        case TokenType::NEGATOR:
            return "NEGATOR";
        case TokenType::ASSIGNER:
            return "ASSIGNER";
        case TokenType::COMPARATOR:
            return "COMPARATOR";
        case TokenType::SEPARATOR:
            return "SEPARATOR";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::ARRAY:
            return "ARRAY";
        case TokenType::KEYWORD:
            return "KEYWORD";
        case TokenType::L_BRACKET:
            return "L_BRACKET";
        case TokenType::R_BRACKET:
            return "R_BRACKET";
        case TokenType::SL_BRACKET:
            return "SL_BRACKET";
        case TokenType::SR_BRACKET:
            return "SR_BRACKET";
        case TokenType::NONETYPE:
            return "NONETYPE";
        case TokenType::SOL:
            return "SOL";
        case TokenType::EOL:
            return "EOL";
        case TokenType::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

ostream& operator<<(ostream& o, const TokenType& tt)
{
    return o << tokentype_to_string(tt);
}

string operator+(string a, TokenType tt)
{
    return a + tokentype_to_string(tt);
}

ostream& operator<<(ostream& o, const Value& val)
{
    return o << "Value(\"" << val.value << "\", " << val.t_type << ")";
}

bool operator==(Value val, Value val2)
{
    return val.t_type == val2.t_type && val.value == val2.value;
}