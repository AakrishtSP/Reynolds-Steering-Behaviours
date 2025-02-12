//#version 460 core
//    out vec4 FragColor;
//void main() {
//    FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green color
//}

#version 460 core

in vec3 vColor; // Interpolated color from vertex shader
out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0); // Use calculated color
}