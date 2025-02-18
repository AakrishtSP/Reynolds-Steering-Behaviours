#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 100) out; // Emit multiple scanline segments

out vec3 fragColor;

void bresenhamLine(vec2 p0, vec2 p1) {
    vec2 p = p0;
    vec2 d = abs(p1 - p0);
    vec2 s = vec2(sign(p1.x - p0.x), sign(p1.y - p0.y));
    
    bool steep = d.y > d.x;
    if (steep) d = d.yx;
    
    float err = d.x / 2.0;
    
    for (int i = 0; i < int(d.x); i++) {
        vec4 pos = vec4((steep ? p.yx : p), 0.0, 1.0);
        gl_Position = vec4(pos.xy, 0.0, 1.0);
        fragColor = vec3(1.0, 1.0, 1.0); // White color for line
        EmitVertex();
        
        err -= d.y;
        if (err < 0) {
            p.y += s.y;
            err += d.x;
        }
        p.x += s.x;
    }
    EndPrimitive();
}

void main() {
    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;
    vec2 p2 = gl_in[2].gl_Position.xy;

    // Draw the three triangle edges using Bresenham's Algorithm
    bresenhamLine(p0, p1);
    bresenhamLine(p1, p2);
    bresenhamLine(p2, p0);

    // Emit horizontal scanline segments (Scanline Fill)
    for (float y = min(p0.y, min(p1.y, p2.y)); y <= max(p0.y, max(p1.y, p2.y)); y += 0.01) {
        vec4 start = vec4(min(p0.x, min(p1.x, p2.x)), y, 0.0, 1.0);
        vec4 end   = vec4(max(p0.x, max(p1.x, p2.x)), y, 0.0, 1.0);
        
        gl_Position = start;
        fragColor = vec3(0.2, 0.8, 0.3); // Green fill color
        EmitVertex();
        
        gl_Position = end;
        fragColor = vec3(0.2, 0.8, 0.3);
        EmitVertex();
        
        EndPrimitive();
    }
}
