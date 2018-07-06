#version 330 core
out vec4 FragColor;

//in vec4 vertexColor;
//uniform vec4 ourColor;
in vec3 v3Color;

void main()
{
    //FragColor = vertexColor;
    //FragColor = ourColor;
    FragColor = vec4(v3Color, 1.0);
}
