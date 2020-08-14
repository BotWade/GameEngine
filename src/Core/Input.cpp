#include "Input.hpp"

map<int, int> Input::Keyboard = map<int, int>();
map<int, int> Input::Mouse = map<int, int>();
Vector2 Input::MousePosition = Vector2();
Vector2 Input::LastMousePosition = Vector2();
Vector2 Input::MouseOffset = Vector2();
Vector2 Input::ScrollOffset = Vector2();

void Input::Clear() {

    MouseOffset.X = 0;
    MouseOffset.Y = 0;
    ScrollOffset.X = 0;
    ScrollOffset.Y = 0;

    for (map<int, int>::iterator it = Mouse.begin(); it != Mouse.end(); ++it)
        if (it->second == GLFW_PRESS)
            it->second++;
}

bool Input::GetKey(int KeyCode) {

    return Keyboard.count(KeyCode) && (Keyboard[KeyCode] == GLFW_REPEAT || Keyboard[KeyCode] == GLFW_PRESS);
}

bool Input::GetKeyDown(int KeyCode) {

    return Keyboard.count(KeyCode) && Keyboard[KeyCode] == GLFW_REPEAT;
}

bool Input::GetKeyUp(int KeyCode) {

    return Keyboard.count(KeyCode) && Keyboard[KeyCode] == GLFW_RELEASE;
}

bool Input::GetMouseButton(int KeyCode) {
    
    return Mouse.count(KeyCode) && Mouse[KeyCode] == GLFW_PRESS;
}

bool Input::GetMouseButtonDown(int KeyCode) {

    return Mouse.count(KeyCode) && Mouse[KeyCode] == GLFW_PRESS + 1;
}

bool Input::GetMouseButtonUp(int KeyCode) {

    return Mouse.count(KeyCode) && Mouse[KeyCode] == GLFW_RELEASE;
}

void Input::InputClear() {
    Keyboard.clear();
    Mouse.clear();
}