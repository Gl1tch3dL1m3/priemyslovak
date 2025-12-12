#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include <string>
using namespace std;

template<typename T, typename T2>
bool is_in_array(T obj, T2 array, const int size)
{
    for (int i {0}; i < size; i++) if (array[i] == obj) return true;
    return false;
}
int count_string(char obj, string str);
string upper(string str);
bool is_numerical(string str);
string trim(string str);

#endif