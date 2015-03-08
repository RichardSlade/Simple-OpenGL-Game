#version 330 core

in vec4 vPosition;
in vec4 vColour;
in vec3 vNormal;

uniform vec3 LightPosition;
uniform mat4 MVP;
uniform mat4 MV;
//uniform vec3 CameraPos;

out vec4 Colour;
out vec3 Normal;
out vec3 LightDirection;
out vec3 ViewDirection;
out vec3 HalfVector;
out vec3 ReflectionVector;

void main()
{
	gl_Position = MVP * vPosition;

	// Transform the normal, without perspective, and normalise it
	Normal = normalize(MV * vec4(vNormal, 0)).xyz;

//	LightDirection = normalize((MV * vec4(1.0, 0.0, 0.0, 0))).xyz;
	LightDirection = normalize(MV * vec4(-LightPosition, 0)).xyz;
//	LightDirection = vec3(1.0, 0.0, 0.0, 0);
//	ViewDirection = normalize(vec3(1.0, -1.0, 1.0));
	ViewDirection = normalize(MV * vPosition).xyz;
//	HalfVector = normalize(LightDirection + ViewDirection);
	HalfVector = normalize(ViewDirection + LightDirection);
	ReflectionVector = reflect(-LightDirection, Normal);

	Colour = vColour;
}

