#ifndef _UPDATEMANAGER_
#define _UPDATEMANAGER_

#include <vector>
#include "UpdateGroup.hpp"

using namespace std;

class UpdateManager {
public:
    static vector<UpdateGroup*> Groups;

    static void CreateGroup(string Name);
    static void AddToGroup(string Name, Object* object);
    static void Update();
    static void Clear();

};

#endif