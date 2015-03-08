#version 330 core

in vec4 vPosition;
in vec2 vUV;
in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform vec3 LightPosition;

out vec2 UV;
out vec3 Normal;
out vec3 LightDirection;
out vec3 ViewDirection;
out vec3 HalfVector;

void main()
{
	gl_Position = MVP * vPosition;

	// Transform the normal, without perspective, and normalise it
	Normal = normalize(MV * vec4(vNormal, 0)).xyz;

	LightDirection = normalize(MV * vec4(-LightPosition, 0)).xyz;
	ViewDirection = normalize(MV * vPosition).xyz;
	HalfVector = normalize(ViewDirection + LightDirection);

	UV = vUV;
}

