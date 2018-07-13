#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 outPos;

uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
//    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
//    FragColor = texture(texture1, texCoord) * vec4(outPos.x*0.5 + 0.5, outPos.y*0.5 + 0.5, outPos.z*0.5 + 0.5, 1.0f);
    FragColor = texture(texture1, texCoord);
}
