#version 330 core

// Output color for the fragment.
out vec4 color;

// Uniform color for the circle outline.
uniform vec4 u_Color;

void main()
{
    // Set the fragment color.
    color = u_Color;
}
