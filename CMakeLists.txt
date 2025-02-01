cmake_minimum_required(VERSION 3.30)
project(GraphicsProject)

# Set C++ standard and output directories
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)

# Recursively find all .cpp and .h files in the src directory
file(GLOB_RECURSE SOURCES
        src/*.cpp
        src/*.h
)

# External dependencies:
# Add GLM, GLFW, and GLEW as subdirectories (assuming they have CMake support)
add_subdirectory(external/glm)
add_subdirectory(external/glfw)
# For GLEW, ensure that its CMake build (or a proper CMakeLists.txt) is available
add_subdirectory(external/glew/build/cmake)

# Create main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for the main target
target_include_directories(${PROJECT_NAME} PUBLIC
        src
        ${CMAKE_SOURCE_DIR}/external/glm
        ${CMAKE_SOURCE_DIR}/external/glfw/include
        ${CMAKE_SOURCE_DIR}/external/glew/include
)

# (Optional) Precompiled header support if available
if(EXISTS "${CMAKE_SOURCE_DIR}/src/pch.h")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/pch.h")
endif()

# --- Manually add Dear ImGui as a static library ---
# Define the ImGui root directory
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

# List the core ImGui source files and the backend implementations.
# Here we include the GLFW backend and the OpenGL3 renderer backend.
# (Note: Adjust the backend files if you wish to use a different configuration.)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Create the imgui library target and set its include directories
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
# Tell ImGui’s OpenGL3 backend to use GLEW as the loader
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)

# --- Link all dependencies to the main executable ---
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw           # from external/glfw
        GLEW::GLEW     # from external/glew (assuming it provides a target)
        OpenGL::GL     # for OpenGL functions
        glm            # from external/glm
        imgui          # our manually added ImGui library
)

# Optionally, set custom output paths per target
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin/${PROJECT_NAME}
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/obj/${PROJECT_NAME}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib/${PROJECT_NAME}
)
