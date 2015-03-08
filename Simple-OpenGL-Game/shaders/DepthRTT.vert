#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vPosition;

// Values that stay constant for the whole mesh.
uniform mat4 DepthMVP;

void main()
{
	gl_Position =  DepthMVP * vPosition;
}

