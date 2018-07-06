#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

//out vec4 vertexColor;
out vec3 v3Color;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    //vertexColor = vec4(aPos.x + 0.5, aPos.y + 0.5, 0.5, 1.0);
    v3Color = aColor;
}
