#ifndef _TIMEHELPER_
#define _TIMEHELPER_

#include <time.h>

class TimeHelper {
    
private:
    static float LastTime;
    static float DeltaTime;
    
public:
static float GetTicks() { return (float)clock() / (float)CLOCKS_PER_SEC; }
    static float GetDeltaTime() { return DeltaTime; } 
    static void Start() { LastTime = GetTicks(); }
    static void Update() { DeltaTime = (GetTicks() - LastTime); LastTime = GetTicks(); }
};
#endif