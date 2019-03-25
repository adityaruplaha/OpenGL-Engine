#version 330 core
in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D texture;

void main()
{
    FragColor = texture(texture, texCoords) * vec4(color.xyz, 1.0f);
} 