#ifndef _WINDOW_
#define _WINDOW_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Debug.hpp"
#include "Input.hpp"
#include "../Math/Matrix4.hpp"
class Window {
public:
    static GLFWwindow* window;
    static Matrix4 OrthoProjection;
    static bool frameBufferResized;
    
    static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
    static int Setup();
    static Vector2 GetSize();
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void errorCallback(int error_code, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void Clear();
};

#endif