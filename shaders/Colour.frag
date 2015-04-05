#version 330 core

in vec4 Colour;

layout(location = 0) out vec4 FragColour;

void main()
{
	FragColour = Colour;
}
