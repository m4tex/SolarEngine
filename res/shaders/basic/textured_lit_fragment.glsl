#version 410 compatibility

in vec2 g_TexCoord;
in vec3 g_Normal;

out vec4 color;

uniform vec3 u_Ambient;
uniform vec3 u_LightDir;

uniform sampler2D u_Texture;

void main()
{
    float diffuse = max(0.0, dot(g_Normal, normalize(u_LightDir)));

    vec4 texColor = texture(u_Texture, g_TexCoord);

    color = texColor * vec4(vec3(diffuse), 1.0) + vec4(u_Ambient, 1.0);
}