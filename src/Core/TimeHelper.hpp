#ifndef _TIMEHELPER_
#define _TIMEHELPER_

#include <time.h>
#include <ctime>
#include <chrono>

using namespace std::chrono;
using ms = std::chrono::duration<float, std::milli>;

class TimeHelper {
    
private:
    static std::chrono::system_clock::time_point LastTime;
    static float DeltaTime;
public:
    static float GetDeltaTime() { return DeltaTime / 1000.0f; } 
    static void Start() { LastTime = high_resolution_clock::now(); }
    static void Update() { DeltaTime = duration_cast<ms>(high_resolution_clock::now() - LastTime).count(); LastTime = high_resolution_clock::now(); }
};
#endif