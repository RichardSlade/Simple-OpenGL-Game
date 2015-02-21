#version 330 core

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal;

out vec4 Position;
out vec2 UV;
out vec3 Normal;

void main()
{
	UV = VertexUV;
	Normal = normalize(NormalMatrix * VertexNormal);
	Position = MVMatrix * VertexPosition;

	gl_Position = MVPMatrix * VertexPosition;
}


