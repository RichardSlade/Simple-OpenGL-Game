#version 330 core

in vec2 UV;

out vec4 FragColour;

uniform sampler2D Tex2DSampler;

void main()
{
	FragColour = texture2D(Tex2DSampler, UV);
//	FragColour = vec4(1, 0 , 1, 1);
}

