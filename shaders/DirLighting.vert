#version 330 core

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat4 ShadowMatrix;

layout(location = 0) in vec4 vPosition;
//layout(location = 1) in vec4 vColour;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;

//out vec4 Colour;

out VS_FS_INTERFACE
{
	vec4 shadowCoord;
//	vec4 colour;
	vec2 uv;
	vec3 worldCoord;
	vec3 eyeCoord;
	vec3 normal;
} Vertex;

void main()
{
	vec4 worldPos = M * vPosition;
	vec4 eyePos = V * worldPos;
	vec4 clipPos = P * eyePos;

	Vertex.shadowCoord = ShadowMatrix * worldPos;
//	Vertex.colour = vColour;
	Vertex.uv = vUV;
	Vertex.worldCoord = worldPos.xyz;
	Vertex.eyeCoord = eyePos.xyz;
	Vertex.normal = normalize(mat3(V * M) * vNormal);

	gl_Position = clipPos;
//	Colour = vColour;
}
