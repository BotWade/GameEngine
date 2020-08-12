#ifndef _INTERFACEUPDATEMANAGER_
#define _INTERFACEUPDATEMANAGER_

#include "InterfaceObject.hpp"
#include "../Core/ObjectsHandler.hpp"

#include <vector>

using namespace std;

class InterfaceManager {
public:
    static InterfaceObject* Canvas;
    static vector<InterfaceObject*> Objects;

    static void Init();
    static void AddObject(InterfaceObject* Object);
    static void ExecuteCode(ExecutionCode code);
};

#endif