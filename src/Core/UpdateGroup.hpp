#ifndef _UPDATEGROUP_
#define _UPDATEGROUP_

#include <string>
#include <vector>
#include <thread>
#include "../Object/Object.hpp"

using namespace std;
using namespace std::chrono;

class UpdateGroup {
public:
    bool Working;
    bool Update;
    string Name;
    thread thrd;
    vector<Object*> Objects;
    std::chrono::nanoseconds UpdateTime;
    
    void Init();
    void Work();
    ~UpdateGroup();
};

#endif