#ifndef _OBJECTSHANDLER_
#define _OBJECTSHANDLER_

#include <vector>
#include <map>

using namespace std;

enum ExecutionCode {
    LOAD,
    UPDATE,
    RENDER,
    POSRENDER
};

class Object;

class ObjectsHandler {    
public:
    static vector<Object*> Objects;
    
    static unsigned int DestroyedObjectIndex;

    static void Load();
    static void ExecuteCode(ExecutionCode code);
    static void AddObject(Object* object);
};

#endif