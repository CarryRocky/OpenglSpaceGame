#version 330 core
out vec4 FragColor;

in vec3 v3Color;
in vec2 texCoord;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord) * vec4(v3Color, 1.0);
}
