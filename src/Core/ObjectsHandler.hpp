#ifndef _OBJECTSHANDLER_
#define _OBJECTSHANDLER_

#include <vector>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

enum ExecutionCode {
    LOAD,
    UPDATE,
    PRERENDER,
    RENDER,
    POSRENDER,
    CLEAN
};

class Object;

class ObjectsHandler {    
public:
    static vector<Object*> Objects;
    
    static unsigned int DestroyedObjectIndex;
    static nanoseconds UpdateTime;
    static nanoseconds RenderTime;
    static nanoseconds PosRenderTime;
    static bool AlreadyLoaded;

    static void Load();
    static void ExecuteCode(ExecutionCode code);
    static void AddObject(Object* object);
    static void Clear();
};

#endif