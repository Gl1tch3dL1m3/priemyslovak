#ifndef STATUSES_H
#define STATUSES_H

#include <string>
#include <iostream>
using namespace std;

enum class Status
{
    OK = 0,
    SYNTAXERROR,
    MATHERROR,
    MEMORYERROR,
    ERROR
};

ostream& operator<<(ostream& o, Status& st);
void make_err(Status st, string err, int col=0);

#endif