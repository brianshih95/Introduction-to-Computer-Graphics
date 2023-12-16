#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform float squeezeFactor;
uniform bool scale;
uniform bool ripple;

out vec2 texCoord;
out vec3 normal;

vec4 worldPos;

void main()
{
	// TODO: Implement squeeze effect and update normal transformation
	//   1. Adjust the vertex position to create a squeeze effect based on squeezeFactor.
	//   2. Update worldPos using the model matrix (M).
	//   3. Calculate the final gl_Position using the perspective (P), view (V), and updated worldPos.
	//   4. Update the normal transformation for lighting calculations.
	// Note: Ensure to handle the squeeze effect for both y and z coordinates.
	vec3 newPos = aPos;

	if (scale)
		newPos *= vec3(1.3f, 1.3f, 0.8f);

	if (ripple) {
        float distance = length(newPos.xy);
        newPos.z += 10 * sin(radians(distance * 5));
    }

	newPos.y += aPos.z * sin(radians(squeezeFactor)) / 2.0;
	newPos.z += aPos.y * sin(radians(squeezeFactor)) / 2.0;
	worldPos = M * vec4(newPos, 1.0);
	gl_Position = P * V * worldPos;
	normal = mat3(transpose(inverse(M))) * aNormal;
	texCoord = aTexCoord;
}