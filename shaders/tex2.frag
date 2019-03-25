#version 330 core
in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

uniform float ratio;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture(texture0, texCoords), texture(texture1, texCoords), ratio) * vec4(color.xyz, 1.0f);
}