#version 330 core

uniform sampler2D ourTexture;

in vec3 fragposGS;
in vec3 normalGS;
in vec2 texCoordGS;

//in vec3 color;

out vec4 FragColor;

void main()
{
    vec3 furColor = texture(ourTexture, texCoordGS).xyz;
    FragColor = vec4(furColor, 1.0);
}