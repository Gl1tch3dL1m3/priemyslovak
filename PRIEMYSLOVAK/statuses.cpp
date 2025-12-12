#include "statuses.h"
#include "interpreter.h"
#include <string>
#include <iostream>
using namespace std;

ostream& operator<<(ostream& o, Status& st)
{
    switch (st)
    {
        case Status::OK:
            return o << "OK";
        case Status::SYNTAXERROR:
            return o << "SyntaxError";
        case Status::MATHERROR:
            return o << "MathError";
        case Status::MEMORYERROR:
            return o << "MemoryError";
        case Status::ERROR:
        default:
            return o << "Error";
    }
}

void make_err(Status st, string err, int col)
{
    cout << st << ": " << err << "! Line: " << ln << endl << "Terminating..." << endl; // << ", Column: " << col
    exit(0);
}