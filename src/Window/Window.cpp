// src/Window/Window.cpp
#include "Window.h"
#include "pch.h"
#include "glad/gl.h"

static void error_callback(const int error, const char* description)
{
    std::cerr << "Error[" <<error<<"]: "<< description << std::endl;
}

static void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


Window::Window(): m_window(nullptr), m_width(0), m_height(0)
{
}

Window::~Window()
{
    shutdown();
}

void Window::init()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(640, 480, "Reynolds Steering Behaviours", nullptr, nullptr);
    if (!m_window)
    {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        assert(false);
    }

    glfwSetKeyCallback(m_window, key_callback);

    glfwMakeContextCurrent(m_window);
    const int version = gladLoadGL(glfwGetProcAddress);
    if ( version == 0) {
        printf("Failed to initialize OpenGL context\n");
        assert(false);
    }
    std::cout << "Loaded OpenGL" << GLAD_VERSION_MAJOR(version)<<"."<< GLAD_VERSION_MINOR(version) << std::endl;

    glfwSwapInterval(1); // Enable vsync
}

void Window::updateStart()
{
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);

}

void Window::updateEnd() const
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::shutdown() const
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
