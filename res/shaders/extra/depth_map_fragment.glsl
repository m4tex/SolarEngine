#version 410 core

in vec2 v_TexCoords;

uniform sampler2D u_DepthMap;

out vec4 color;

void main() {
    float depthValue = texture(u_DepthMap, v_TexCoords).r;

    float linearDepth = 1 / (1000.001 - depthValue * 999.999);

    color = vec4(vec3(linearDepth), 1.0);
}