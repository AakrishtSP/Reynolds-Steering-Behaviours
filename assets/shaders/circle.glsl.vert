#version 330 core

// Input vertex position (x, y, z)
layout(location = 0) in vec3 a_Position;

// Uniform Model-View-Projection matrix.
uniform mat4 u_MVP;

void main()
{
    // Transform the vertex position by the MVP matrix.
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}