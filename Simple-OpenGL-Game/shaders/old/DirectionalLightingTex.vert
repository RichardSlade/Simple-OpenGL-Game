#version 330 core

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 NormalMatrix;
//uniform vec3 LightPosition;
//uniform vec3 ViewDirection;

in vec4 vPosition;
in vec2 vUV;
in vec3 vNormal;

out vec2 UV;
out vec3 Normal;
//out vec3 HalfVector;
//out vec3 LightDirection;

void main()
{
	UV = vUV;
	Normal = normalize(NormalMatrix * vNormal);
//	Normal = normalize(MV * vec4(vNormal, 0)).xyz;

//	vec3 ViewDirection = normalize(MV * vPosition).xyz;

//	LightDirection = normalize(MV * vec4(-LightPosition, 0)).xyz;
//	LightDirection = normalize(vec4(-LightPosition, 0)).xyz;
//	HalfVector = normalize(ViewDirection + LightDirection);
//	HalfVector = normalize(LightDirection + ViewDirection);

	gl_Position = MVP * vPosition;
}

