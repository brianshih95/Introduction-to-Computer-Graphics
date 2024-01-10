#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT{
    vec2 texCoord;
} gs_in[];

out GS_OUT {
    vec4 color;
} gs_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform float magnitude;
uniform float scaleFactor;
uniform float time;
uniform int squirt;
uniform int onFire;

uniform sampler2D ourTexture;

void main() {

    vec4 positions[gl_in.length()];
    for (int i = 0; i < gl_in.length(); i++)
        positions[i] = P * V * M * gl_in[i].gl_Position;

    vec3 edge1 = positions[0].xyz - positions[1].xyz;
    vec3 edge2 = positions[2].xyz - positions[1].xyz;
    vec3 normal = normalize(cross(edge1, edge2));

    for (int i = 0; i < gl_in.length(); i++){
        gl_Position = positions[i] * vec4(scaleFactor, scaleFactor, 1, 1);
        gl_Position += vec4(normal, 0.0f) * magnitude;
        gs_out.color = texture(ourTexture, gs_in[i].texCoord);
        EmitVertex();

        if (squirt == 1 && 
            positions[i].x < -1.1 && positions[i].x > -1.15 &&
            positions[i].y > 1.4 && positions[i].y < 1.6 && 
            positions[i].z > 3.3 && positions[i].z < 3.5)
        {
            gl_Position = positions[i] * vec4(scaleFactor, scaleFactor, 1, 1) + 
            vec4(normal, 0.0f) * scaleFactor * abs(sin(time)) * 5;
            gs_out.color = vec4(1, 1, 1, 1);
            EmitVertex();
        }
        else if (onFire == 1) {
            gl_Position = positions[i];
            gs_out.color = vec4(1, 0.5, 0, 1);
            EmitVertex();
        }
    }

    EndPrimitive();
    
}
