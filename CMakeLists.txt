cmake_minimum_required(VERSION 3.29)
project(ReynoldsSteering)


# Enforce clang-tidy checks if available
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    # Option 1: Let clang-tidy automatically pick up the .clang-tidy file
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-header-filter=^${CMAKE_SOURCE_DIR}/src/.*")

    # Option 2: Force clang-tidy to use the .clang-tidy file in the project root.
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-config=file:${CMAKE_SOURCE_DIR}/.clang-tidy")
endif()


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

message(STATUS "Found C++ files:")
foreach(FILE ${SOURCES})
  message(STATUS "  ${FILE}")
endforeach()

# External dependencies:
# Add GLM, GLFW, and GLEW as subdirectories (assuming they have CMake support)
add_subdirectory(external/glm)
add_subdirectory(external/glfw)

# Create main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for the main target
target_include_directories(${PROJECT_NAME} PUBLIC
        src
        ${CMAKE_SOURCE_DIR}/external/glm
        ${CMAKE_SOURCE_DIR}/external/glfw/include
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

# (Optional) Precompiled header support if available
if (EXISTS "${CMAKE_SOURCE_DIR}/src/pch.h")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/pch.h") 
endif ()

#--- Manually add GLAD as a static library ---
# Define the GLAD root directory
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/external/glad)

# Create the glad library target
add_library(glad STATIC ${GLAD_DIR}/src/gl.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)


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
        ${CMAKE_SOURCE_DIR}/external/glfw/include
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
# Tell ImGui’s OpenGL3 backend to use GLAD as the loader
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC GLFW_INCLUDE_NONE
        PUBLIC GLAD_GL_IMPLEMENTATION
)

# --- Link all dependencies to the main executable ---
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw           # from external/glfw
        glad           # our manually added GLAD library
        glm            # from external/glm
        imgui          # our manually added ImGui library
)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

# Optionally, set custom output paths per target
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/obj
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib
)


# Copy assets to the build directory after building Examples
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)
