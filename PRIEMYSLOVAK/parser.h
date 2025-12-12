#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

void parser(vector<Token> &tokens, int start_i, int end_i, bool is_starting = false);
Token& get_token(vector<Token> &tokens, int i);
Token& get_next_type_token(vector<Token> &tokens, TokenType tt, int i, int step=0);
Token& get_prev_type_token(vector<Token> &tokens, TokenType tt, int i);
Token& get_token_ranged(vector<Token> &tokens, int token_pos, int &start_i, int &end_i);

#endif