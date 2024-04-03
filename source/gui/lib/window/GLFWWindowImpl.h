#pragma once

#include <string>
#include <cstring> // For strncpy
#include <queue>
#include <GLFW/glfw3.h>
#include "WindowImpl.h"

class Window;

class GLFWWindowImpl : public WindowImpl {
public:
    // Constructor
    GLFWWindowImpl(Window* window, const std::string& title, int width, int height);

    // Destructor
    virtual ~GLFWWindowImpl();

    // Event handling
    bool pollEvent(Event& event) override;
    void glfwGetEvents();

    // Getters
    GLFWwindow* getGlfwWindow() const override;
    //void* getNativeHandle() const override;
    //WindowingSystem getWindowingSystem() const override;
    static GLFWWindowImpl* getCallingWindow(GLFWwindow* window);

    // Callback
    static void glfwErrorCallback(int error, const char* description);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwCharCallback(GLFWwindow* window, unsigned int codepoint);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void glfwWindowCloseCallback(GLFWwindow* window);
    static void glfwWindowIconifyCallback(GLFWwindow* window, int iconified);
    static void glfwWindowMaximizeCallback(GLFWwindow* window, int maximized);
    static void glfwWindowRefreshCallback(GLFWwindow* window);
    static void glfwWindowFocusCallback(GLFWwindow* window, int focused);
    static void glfwWindowPosCallback(GLFWwindow* window, int x, int y);
    //static void glfwJoystickCallback(int joy, int event);

protected:
    void init() override;
    void createNative();
    void initOpenGL();
    void initBGFX();
    void initEmpty();
    void init_handle() override;

    // Setters
    virtual void setTitleUnsafe(char* title) override;
    virtual void setSizeUnsafe(int width, int height) override;

private:
    GLFWwindow* m_glfw_window = 0;
};