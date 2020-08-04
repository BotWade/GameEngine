#ifndef _UNIFORM_
#define _UNIFORM_

#include <string>

using namespace std;

struct Uniform {
    string Name;
    unsigned int Id;

    Uniform(string name);
};


#endif