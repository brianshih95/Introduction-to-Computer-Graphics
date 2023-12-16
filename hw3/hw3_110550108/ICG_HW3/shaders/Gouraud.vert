#version 330 core

// TODO:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec2 texCoord;
out vec4 ambient;
out vec4 diffuse;
out vec4 specular;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0f);
	texCoord = aTexCoord;
	vec4 worldPos = M * vec4(aPos, 1.0f);
	vec3 normal = mat3(transpose(inverse(M))) * aNormal;

	vec4 Ka = vec4(material.ambient, 1.0f);
	vec4 Kd = vec4(material.diffuse, 1.0f);
	vec4 Ks = vec4(material.specular, 1.0f);
	vec4 La = vec4(light.ambient, 1.0f);
	vec4 Ld = vec4(light.diffuse, 1.0f);
	vec4 Ls = vec4(light.specular, 1.0f);

	vec3 L = normalize(light.position - worldPos.xyz);
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos - worldPos.xyz);
	vec3 R = normalize(reflect(-L, N));

	ambient =  La * Ka;
	diffuse = Ld * Kd * max(dot(L, N), 0);
	specular = Ls * Ks * pow(max(dot(V, R), 0), material.shininess);
}