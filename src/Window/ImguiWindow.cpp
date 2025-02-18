// src/Window/ImguiWindow.cpp
#include "ImguiWindow.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ImguiWindow::ImguiWindow(): m_window(nullptr)
{
}

void ImguiWindow::init(Window* window)
{
    m_window = window;
    // Setup Dear ImGui context
    const auto glsl_version = "#version 460";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    (void)m_io;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io->ConfigViewportsNoAutoMerge = true;
    //io->ConfigViewportsNoTaskBarIcon = true;

    // ✅ Only enable Viewports if not running on Wayland
    if ((std::getenv("XDG_SESSION_TYPE") && std::string(std::getenv("XDG_SESSION_TYPE")) != "wayland") || !std::getenv(
        "XDG_SESSION_TYPE"))
    {
        m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImguiWindow::updateStart() const
{
    if (glfwGetWindowAttrib(m_window->getWindow(), GLFW_ICONIFIED) != 0)
    {
        glfwWaitEvents();
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    IM_ASSERT(ImGui::GetCurrentContext() != nullptr && "ImGui context is NULL!"); // Debugging check
    ImGui::NewFrame();
}

void ImguiWindow::updateEnd() const
{
    ImGui::Begin("FPS debug");
    ImGui::Text("Current FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Current Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImguiWindow::shutdown()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
