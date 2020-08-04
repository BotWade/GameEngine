#ifndef _WINDOW_
#define _WINDOW_

#include <GLFW/glfw3.h>

#include "Debug.hpp"
#include "Input.hpp"
#include "../Math/Vector2.hpp"

class Window {
public:
    static GLFWwindow* window;

    static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
    static int Setup();
    static Vector2 GetSize();
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void errorCallback(int error_code, const char* description);
};

#endif