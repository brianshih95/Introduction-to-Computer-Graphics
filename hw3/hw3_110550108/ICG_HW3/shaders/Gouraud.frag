#version 330 core

// TODO:
// Implement Gouraud shading

in vec2 texCoord;
in vec4 ambient;
in vec4 diffuse;
in vec4 specular;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	vec4 objColor = texture(ourTexture, texCoord);
	FragColor = ambient * objColor + diffuse * objColor + specular;
}