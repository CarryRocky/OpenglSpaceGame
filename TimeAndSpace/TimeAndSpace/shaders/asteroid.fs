#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Light {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

uniform sampler2D texture_diffuse1;

uniform vec3 lightSource;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));
    
    FragColor = texture(texture_diffuse1, TexCoords) * diff * attenuation;
}
