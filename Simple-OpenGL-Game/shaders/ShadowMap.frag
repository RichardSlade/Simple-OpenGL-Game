#version 330 core

in vec2 UV;

out vec4 FragColour;

//uniform sampler2D Tex2DSampler;

void main()
{
//	float depth = texture2D(Tex2DSampler, UV).x;
//	depth = 1.0 - (1.0 - Depth) * 25.0;
//	FragColour = vec4(Depth);

	FragColour = vec4(1, 1, 0, 1);
}
