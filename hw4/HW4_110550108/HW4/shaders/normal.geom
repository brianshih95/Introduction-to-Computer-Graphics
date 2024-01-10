#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;
//Input/Output part is important, so be sure to check how everything works

in VS_OUT {
	vec3 normal;
	vec3 fragpos;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;
uniform vec3 windshift;


out vec3 fragposGS;
out vec3 normalGS;
out vec2 texCoordGS;

//out vec3 color;

const float FUR_LENGTH = 0.065;

//This is the most important part we want you to implement on yourself
//Use demo code for a learning example and design a geometry shader for your object

void main()
{
    for (int i = 0; i < gl_in.length(); ++i) {

	    fragposGS = gs_in[i].fragpos;
        normalGS = normalize(gs_in[i].normal);
		texCoordGS = gs_in[i].texCoord;

		//A very simple fur generator (and can definitely be improved with effort)
        gl_Position = P * gl_in[i].gl_Position;
	    EmitVertex();
        gl_Position = P * gl_in[i].gl_Position + vec4(gs_in[i].normal , 0.0)*FUR_LENGTH + vec4(windshift,0.0);
        EmitVertex();

		//Try to put EndPrimitive() here and change triangle_strip to line_strip to see what's going on
		//That might be helpful to achieve more creativity goals
		}
	EndPrimitive();
  
}