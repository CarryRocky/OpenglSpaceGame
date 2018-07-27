#version 330 core
out vec4 FragColor;

in vec3 texCoord;
in vec3 outPos;

uniform samplerCube texture1;

void main()
{
    FragColor = texture(texture1, texCoord);
}
