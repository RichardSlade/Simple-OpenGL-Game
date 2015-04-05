#version 330 core

in vec2 UV;

out vec4 FragColour;

uniform sampler2D Tex2DSampler;

void main()
{
//	FragColour = texture2D(Tex2DSampler, UV);
	float depth = texture2D(Tex2DSampler, UV).z;

	depth = 1.0 - (1.0 - depth) * 25.0;

	FragColour = vec4(depth);

//	FragColour = vec4(1, 1, 0, 1);
}

