#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 rotation;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = vec3(rotation * vec4(aPos, 1.0));
    FragPos = vec3(model * vec4(aPos, 1.0));
}
