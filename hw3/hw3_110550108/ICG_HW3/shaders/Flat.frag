#version 330 core

// TODO:
// Implement Flat shading

in GS_OUT {
    vec2 texCoord;
    vec3 worldPos;
    vec3 normal;
} fs_in;

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

out vec4 FragColor;

void main()
{
    vec4 objColor = texture(ourTexture, fs_in.texCoord);

    vec4 Ka = vec4(material.ambient, 1.0f);
	vec4 Kd = vec4(material.diffuse, 1.0f);
	vec4 Ks = vec4(material.specular, 1.0f);
	vec4 La = vec4(light.ambient, 1.0f);
	vec4 Ld = vec4(light.diffuse, 1.0f);
	vec4 Ls = vec4(light.specular, 1.0f);

    vec3 L = normalize(light.position - fs_in.worldPos);
	vec3 N = normalize(fs_in.normal);
	vec3 V = normalize(cameraPos - fs_in.worldPos);
	vec3 R = normalize(reflect(-L, N));

	vec4 ambient =  objColor * La * Ka;
	vec4 diffuse = objColor * Ld * Kd * max(dot(L, N), 0);
	vec4 specular = Ls * Ks * pow(max(dot(V, R), 0), material.shininess);
    
    FragColor = ambient + diffuse + specular;
}
