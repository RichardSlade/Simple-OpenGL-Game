#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColour;
layout(location = 2) in vec2 vUV;

out vec4 Colour;
out vec2 UV;

void main()
{
	gl_Position = vPosition;

	Colour = vColour;
	UV = vUV;
}
