#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <map>
using namespace std;

extern int ln;
extern vector<string> lines;
extern map<string, Value> identifiers;
extern map<string, vector<Token>> arrays;
extern vector<string> temp_arrays;
int interpreter(vector<Token> &tokens, int start_i, int end_i, bool is_subprocess, bool is_looping);
void priemyslovak(string cmd, bool is_looping_interpreter);
void run_script(string path);

#endif