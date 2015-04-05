#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vPosition;
//layout(location = 1) in vec2 vUV;
//layout(location = 2) in vec3 vNormal;

// Output data ; will be interpolated for each fragment.
//out vec2 UV;
out vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 DepthBiasMVP;

void main()
{

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vPosition;

	ShadowCoord = DepthBiasMVP * vPosition;

	// UV of the vertex. No special space for this one.
//	UV = vUV;
}

