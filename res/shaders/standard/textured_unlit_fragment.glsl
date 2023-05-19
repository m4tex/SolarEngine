#version 410 compatibility

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    color = vec4(1.0, 1.0, 1.0, 1.0);
}