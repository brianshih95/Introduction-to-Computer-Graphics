#version 330 core

// Advanced:
// Implement Border effect

uniform vec3 cameraPos;
uniform sampler2D ourTexture;

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec4 objColor = texture(ourTexture, texCoord);

	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos - worldPos.xyz);
	
	if (abs(dot(V, N)) < 0.3f)
		FragColor = vec4(0.94f, 0.9f, 0.86f, 1.0f);
	else
		FragColor = objColor;
}
