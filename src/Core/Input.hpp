#ifndef _INPUT_
#define _INPUT_

#include <map>
#include "Window.hpp"
#include "../Math/Vector2.hpp"

using namespace std;

class Input {    
public:

    static map<int, int> Keyboard;
    static map<int, int> Mouse;

    static Vector2 MousePosition;
    static Vector2 MousePositionYInv;
    static Vector2 LastMousePosition;
    static Vector2 MouseOffset;
    static Vector2 ScrollOffset;
    
    static void Clear();
    static void InputClear();
    static bool GetKey(int KeyCode);
    static bool GetKeyDown(int KeyCode);
    static bool GetKeyUp(int KeyCode);
    static bool GetMouseButton(int KeyCode);
    static bool GetMouseButtonDown(int KeyCode);
    static bool GetMouseButtonUp(int KeyCode);
};

#endif