#version 330 core

// TODO:
// Implement Toon shading

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

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

in vec4 worldPos;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec4 Ks = vec4(material.specular, 1.0f);
	vec4 Ls = vec4(light.specular, 1.0f);

	vec3 L = normalize(light.position - worldPos.xyz);
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos - worldPos.xyz);
	vec3 R = normalize(reflect(-L, N));

	vec4 specular = Ls * Ks * pow(max(dot(V, R), 0), material.shininess);

	float threshold = 0.01f;
	vec4 high = vec4(0.85f, 0.78f, 0.73f, 1.0f), medium = vec4(0.5f, 0.33f, 0.26f, 1.0f), low = vec4(0.18f, 0.1f, 0.1f, 1.0f);
	
	if (abs(dot(L, N)) < 0.3f)
		FragColor = low;
	else if (specular.x > threshold)
		FragColor = high;
	else
		FragColor = medium;
}