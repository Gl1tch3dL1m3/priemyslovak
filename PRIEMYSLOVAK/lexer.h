#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <vector>
#include "statuses.h"
using namespace std;

enum class TokenType
{
   INTEGER, // 60
   FLOAT, // 5.74
   STRING, // "basic+", 'basic+'
   BOOL, // TRUE/FALSE
   PLUS, // +
   MINUS, // -
   MULTIPLIER, // *
   DIVISOR, // /
   POWER, // ^
   NEGATOR, // ~
   MODULO, // %
   ASSIGNER, // =
   COMPARATOR, // ==, ~=, >, <, >=, <=
   SEPARATOR, // ,
   KEYWORD, // PRINT, VAR, IF, ENDIF, FUNC, ...
   IDENTIFIER, // (names of variables, functions, ...)
   ARRAY, // token made from []
   L_BRACKET, // (
   R_BRACKET, // )
   SL_BRACKET, // [
   SR_BRACKET, // ]
   NONETYPE, // NULL
   SOL, // Start Of Line/Code
   EOL, // End Of Line/Code
   UNKNOWN,
   ERROR
};

const int op_count {16};
const int keyword_count {28};
const int datatypes_count {5};
const int op_count_parser {8};
const int numerical_count {2};
const int nonnumerical_count {4};
const int starts_sol_count {3};
const int datatype_keywords_count = 5;

const char op_list[op_count] {'+', '-', '*', '/', '^', '~', '=', '~', '<', '>', '(', ')', ',', '[', ']', '%'};

const TokenType datatypes[datatypes_count]
{
   TokenType::INTEGER,
   TokenType::FLOAT,
   TokenType::STRING,
   TokenType::BOOL,
   TokenType::NONETYPE
};

const TokenType op_list_parser[op_count_parser] 
{
   TokenType::PLUS, // num + str
   TokenType::MINUS, // num
   TokenType::MULTIPLIER, // num
   TokenType::DIVISOR, // num
   TokenType::POWER, // num
   TokenType::ASSIGNER, // identifier = datatype
   TokenType::NEGATOR, // num
   TokenType::SEPARATOR
};

const TokenType numerical[numerical_count]
{
   TokenType::INTEGER,
   TokenType::FLOAT
};

const TokenType nonnumerical[nonnumerical_count]
{
   TokenType::STRING,
   TokenType::BOOL,
   TokenType::NONETYPE,
   TokenType::ERROR
};

const string keywords[keyword_count]
{
   "PRINT", //
   "PRINTLN", //
   "INPUT", //
   "IF", //
   "ELIF", //
   "AND", //
   "OR", //
   "END", //
   "ELSE", //
   "GOTO", //
   "THEN", //
   "FOR", //
   "TO", //
   "STEP", //
   "WHILE", //
   "BREAK", //
   "CONTINUE", //
   "DO", //
   //"TRY",
   //"EXCEPT",
   //"RETURN",
   "SLEEP", //
   "RAISE", //
   "IMPORT", //
   "DELETE", //
   "EXECUTE", //
   "TERMINATE", //
   "TO_STRING",
   "TO_INT",
   "TO_FLOAT",
   "RANDOM"
};

const string starts_sol[starts_sol_count]
{
   "THEN",
   "DO",
   "TO",
};

const string datatype_keywords[datatype_keywords_count] // keywords that returns a datatype value
{
   "TO_STRING",
   "TO_INT",
   "TO_FLOAT",
   "RANDOM",
   "INPUT"
};

struct Value
{
   Value();
   Value(string value, TokenType t_type);
   
   string value;
   TokenType t_type;
};

struct Token
{
   Token(Value value, unsigned col);
   
   unsigned col;
   Value value;
};

struct Array
{
   vector<Token> tokens;
   int length = 0;

   template<typename... args>
   Array(args... _args)
   {
      tokens.clear();

      ((__makearr(_args)), ...);
   }

   Token& get(int index, int ln, int col)
   {
      try
      {
         return tokens[index];
      }
      catch(...)
      {
         make_err(Status::MEMORYERROR, "Array index out of range", col);
      }
   }

   private:
      void __makearr(Token& arg)
      {
         tokens.push_back(arg);
         length++;
      }
};

ostream& operator<<(ostream& o, Token& token);
ostream& operator<<(ostream& o, const TokenType& tt);
ostream& operator<<(ostream& o, const Value& val);
string operator+(string a, TokenType tt);
vector<Token> lexer(string str);
bool operator==(Value val, Value val2);

#endif