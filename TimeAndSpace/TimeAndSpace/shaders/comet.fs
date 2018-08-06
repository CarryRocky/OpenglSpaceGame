#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

//uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = vec3(1,1,1) * diff;
    
    FragColor = vec4(diffuse, 1.0);
}
