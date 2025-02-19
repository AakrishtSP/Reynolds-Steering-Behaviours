// src/Window/Window.cpp
#include "Window.h"

#include <thread>

#include "pch.h"
#include "glad/gl.h"
#ifdef _WIN32
#include <windows.h>
#endif


static void error_callback(const int error, const char* description)
{
    std::cerr << "Error[" <<error<<"]: "<< description << std::endl;
}

static void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void APIENTRY OpenGLDebugMessageCallback(const GLenum source, const GLenum type, GLuint id, const GLenum severity,
                                         GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "🔷 [OpenGL Debug Message] 🔷\n";
    std::cerr << "Message: " << message << std::endl;
    std::cerr << "Source: ";

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Other"; break;
    default: std::cerr << "Unknown"; break;
    }
    std::cerr << "\nType: ";

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cerr << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cerr << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cerr << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "Other"; break;
    default: std::cerr << "Unknown"; break;
    }
    std::cerr << "\nSeverity: ";

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "🔴 High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "🟠 Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "🟡 Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "🔵 Notification"; break;
    default: std::cerr << "Unknown"; break;
    }
    std::cerr << "\n----------------------------------------\n";
}

void EnableOpenGLDebugging()
{
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        std::cout << "✅ OpenGL Debug Output Enabled\n";

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        std::cout << "⚠ OpenGL Debug Context Not Available\n";
    }
}


Window::Window(): m_window(nullptr), m_width(0), m_height(0), m_startTime(0), m_endTime(0)
{
}


void Window::init()
{
    // At initialization
#ifdef _WIN32
    timeBeginPeriod(1);  // Request 1ms resolution
#endif
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // ✅ Request Debug Context
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE); // Set the window to be floating

    m_window = glfwCreateWindow(1280, 720, "Reynolds Steering Behaviours", nullptr, nullptr);
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

    glfwSwapInterval(0); // Enable vsync

    EnableOpenGLDebugging();
}

void Window::updateStart()
{
    m_startTime = glfwGetTime();
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::updateEnd()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    m_endTime = glfwGetTime();
    if (const double elapsed = m_endTime - m_startTime; elapsed < m_targetFrameTime)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>(m_targetFrameTime - elapsed));
    }
}

void Window::shutdown() const
{
    // Clean up: Reset the timer resolution.
#ifdef _WIN32
    timeEndPeriod(1);
#endif
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
