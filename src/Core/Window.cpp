#include "Window.hpp"
#include "../InterfaceObject/InterfaceManager.hpp"

GLFWwindow* Window::window;
Matrix4 Window::OrthoProjection;

int Window::Setup() {
    
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    OrthoProjection = Matrix4::Ortho(0, 1280, 0, 720);

    return 0;
}

Vector2 Window::GetSize() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return Vector2(width, height);
}

float firstMouse = true;

void Window::MouseCallback(GLFWwindow* window, double xPos, double yPos) {

    if (firstMouse) {
    
        Input::LastMousePosition.Y = xPos;
        Input::LastMousePosition.X = yPos;
        firstMouse = false;
    }

    Input::LastMousePosition.X = Input::MousePosition.X;
    Input::LastMousePosition.Y = Input::MousePosition.Y;

    Input::MousePosition.X = xPos;
    Input::MousePosition.Y = yPos;

    Input::MouseOffset.X = Input::LastMousePosition.X - Input::MousePosition.X;
    Input::MouseOffset.Y = Input::LastMousePosition.Y - Input::MousePosition.Y;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    Input::Keyboard[key] = action;
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    Input::Mouse[button] = action;
}

void Window::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

    Input::ScrollOffset.X = xoffset;
    Input::ScrollOffset.Y = yoffset;
}

void Window::errorCallback(int error_code, const char* description) {

    Debug::Alert("OpenGL Error - Code" + to_string(error_code) + ": \n" + description);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    InterfaceManager::Canvas->Size = Vector2(width, height);
    OrthoProjection = Matrix4::Ortho(0, width, 0, height);
}

void Window::Clear() {
    glfwDestroyWindow(window);
    glfwTerminate();
}