# Reynolds Steering Behaviors

This project implements Reynolds' Steering Behaviors using OpenGL.

## Cloning the Repository

To ensure all submodules are cloned properly, use the following command:

```sh
git clone --recursive https://github.com/AakrishtSP/Reynolds-Steering-Behaviours.git
```

If you forgot to clone recursively, you can initialize and update submodules manually:

```sh
git submodule update --init --recursive
```

## Building with CMake

### Requirements
- CMake (>=3.30 recommended)
- A C++ compiler with C++17 support
- OpenGL (typically provided by your system or graphics drivers)
- GLFW, GLM, DearImGui, glad (included as submodules)

### Steps to Build

1. Navigate to the project directory:
   ```sh
   cd Reynolds-Steering-Behaviours
   ```

2. Create a build directory:
   ```sh
   mkdir build && cd build
   ```

3. Run CMake to configure the project:
   ```sh
   cmake ..
   ```

4. Compile the project:
   ```sh
   cmake --build . --config Release
   ```
   Or on Unix-based systems:
   ```sh
   make -j$(nproc)
   ```

5. Run the executable:
   ```sh
   ./build/bin/ReynoldsSteering
   ```

### Summary

Ensure you have CMake and a compatible compiler installed. Then, follow these steps:
```sh
mkdir build
cd build
cmake ..
make -j$(nproc)
```

#### On Windows

```sh
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
```
Then, open the generated `.sln` file in Visual Studio and build the project.

Alternatively, use:
```sh
cmake --build . --config Release
```

Run the executable:
```sh
./build/bin/ReynoldsSteering.exe
```

## Troubleshooting

### Submodule Issues
If you run into issues with submodules, try:
```sh
git submodule update --init --recursive
```

### CMake Errors
- If CMake fails to find OpenGL, ensure `GLFW` and other dependencies are correctly set up.
- If using `make`, try running `cmake --build . --config Release` instead.

