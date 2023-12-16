#version 330 core

// TODO:
// Implement Flat shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out VS_OUT{
    vec2 texCoord;
    vec3 worldPos;
} vs_out;

void main()
{
    gl_Position = P * V * M * vec4(aPos, 1.0f);
    vs_out.texCoord = aTexCoord;
    vs_out.worldPos = vec3(M * vec4(aPos, 1.0f));
}
