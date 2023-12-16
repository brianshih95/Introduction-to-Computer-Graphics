#version 330 core

// TODO:
// Implement Blinn-Phong shading

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
uniform sampler2D ourTexture;

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec4 objColor = texture(ourTexture, texCoord);

	vec4 Ka = vec4(material.ambient, 1.0f);
	vec4 Kd = vec4(material.diffuse, 1.0f);
	vec4 Ks = vec4(material.specular, 1.0f);
	vec4 La = vec4(light.ambient, 1.0f);
	vec4 Ld = vec4(light.diffuse, 1.0f);
	vec4 Ls = vec4(light.specular, 1.0f);

	vec3 L = normalize(light.position - worldPos.xyz);
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos - worldPos.xyz);
	vec3 H = normalize(L + V);

	vec4 ambient = objColor * La * Ka;
	vec4 diffuse = objColor * Ld * Kd * max(dot(L, N), 0);
	vec4 specular = Ls * Ks * pow(max(dot(N, H), 0), material.shininess);
	
	FragColor = ambient + diffuse + specular;
}