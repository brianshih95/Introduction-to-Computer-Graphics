#version 330 core

// TODO:
// Implement Flat shading

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
    vec2 texCoord;
    vec3 worldPos;
} gs_in[];

out GS_OUT {
    vec2 texCoord;
    vec3 worldPos;
    vec3 normal;
} gs_out;

void main()
{
    vec3 normal = normalize(cross(gs_in[1].worldPos - gs_in[0].worldPos, gs_in[2].worldPos - gs_in[0].worldPos));

    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        gs_out.texCoord = gs_in[i].texCoord;
        gs_out.worldPos = gs_in[i].worldPos;
        gs_out.normal = normal;
        EmitVertex();
    }
    EndPrimitive();
}
