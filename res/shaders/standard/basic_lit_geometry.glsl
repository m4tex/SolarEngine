#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 g_Normal;

void main() {
    vec3 a = gl_in[0].gl_Position.xyz;
    vec3 b = gl_in[1].gl_Position.xyz;
    vec3 c = gl_in[2].gl_Position.xyz;

    vec3 normal = normalize(cross(b-a, c-a));

    for (int i = 0; i < 3; i++) {
        g_Normal = normal;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}