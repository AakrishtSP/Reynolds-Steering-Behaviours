#version 460 core

layout (location = 0) in vec3 aPos; // Vertex position
//out vec3 vColor; // Output color to fragment shader

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(aPos, 1.0);

    // Generate a color based on the vertex position
//    vColor = aPos * 0.5 + 0.5; // Normalize range [-1,1] to [0,1]
}