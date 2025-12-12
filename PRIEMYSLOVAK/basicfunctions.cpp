#include "basicfunctions.h"
#include <algorithm>

int count_string(char obj, string str)
{
    int count {0};
    for (int i {0}; i < str.length(); i++) if (str[i] == obj) count++;
    return count;
}

string upper(string str)
{
    for (int i {0}; i < str.length(); i++) str[i] = toupper(str[i]);
    return str;
}

bool is_numerical(string str)
{
    bool is_num {false};
    for (char ch : str) if (!isdigit(ch)) { is_num = true; break; }
    return is_num;
}

string trim(string str)
{
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}