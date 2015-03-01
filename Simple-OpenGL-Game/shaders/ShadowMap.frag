#version 330 core

in vec4 Colour;
in vec2 UV;

out vec4 FragColour;

uniform sampler2D Tex2DSampler;

void main()
{
	float depth = texture2D(Tex2DSampler, UV).x;
//	FragColour = texture2D(Tex2DSampler, UV);
	depth = 1.0 - (1.0 - depth) * 25.0;

//   vec3 depthColour = vec3(depth);

//	FragColour = vec4(depthColour, 1);

	FragColour = vec4(depth);
//	FragColour = Colour;
}
