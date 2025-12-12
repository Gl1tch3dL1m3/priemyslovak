/*

------ PRIEMYSLOVAK ------
Vitajte v zdrojovom kóde programovacieho jazyka PRIEMYSLOVAK!
Príručku používania nájdete v zložke "documents".

Je kód chaotický? To je len ilúzia :p

*/

#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "basicfunctions.h"
using namespace std;

const string _NAME_ = "PRIEMYSLOVAK";
const string _AUTHOR_ = "glitchedlime";
const string _VERSION_ = "1.0";
const string _YEAR_ = "2025";

int ln = 0;
vector<string> lines;
map<string, Value> identifiers;
map<string, vector<Token>> arrays;
vector<string> temp_arrays;

/*

POZNÁMKY:
- DELETE a[0][1]

- spraviť [1,2,3] == [1,2,3]
*/

int main(int argc, char* argv[])
{
    vector<string> args {argv, argv + argc};
    string cmd;
    
    if (argc == 1)
    {
        cout << "---- " + _NAME_ + " ----\n(C) " + _YEAR_ + " " + _AUTHOR_ + "\nVerzia: " + _VERSION_ + "\n";
    
        while (true)
        {
            cout << "\n>>> ";

            getline(cin, cmd);
            lines.push_back(cmd);
            ln++;
            priemyslovak(cmd, false);
        }
    }

    else if (argc == 2)
    {
        run_script(argv[1]);
    }

    return 0;
}