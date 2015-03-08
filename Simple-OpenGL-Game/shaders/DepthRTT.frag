#version 330 core

// Ouput data
layout(location = 0) out float fragDepth;

void main()
{
	fragDepth = gl_FragCoord.z;
//	fragDepth = 1.0 - (1.0 - fragDepth) * 25.0;

//	fragDepth = 1.f;
}
