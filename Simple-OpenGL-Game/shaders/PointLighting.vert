#version 330 core

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NormalMatrix;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;

out vec4 Position;
out vec2 UV;
out vec3 Normal;

void main()
{
	UV = vUV;
	Normal = normalize(NormalMatrix * vNormal);
	Position = MV * vPosition;

	gl_Position = MVP * vPosition;
}


