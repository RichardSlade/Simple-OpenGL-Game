#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vUV;

void main()
{
	gl_Position = vPosition;

	UV = vUV;
}
