#version 330 core

// Interpolated values from the vertex shaders
//in vec2 UV;
in vec4 ShadowCoord;

// Ouput data
layout(location = 0) out vec4 fragColour;

// Values that stay constant for the whole mesh.
uniform sampler2D Tex2DSampler;
uniform sampler2DShadow ShadowSampler;

void main(){

	// Light emission properties
	vec3 LightColour = vec3(1,1,1);

	// Material properties
//	vec3 MaterialDiffuseColor = texture2D(Tex2DSampler, UV).rgb;

	vec3 MaterialDiffuseColour = vec3(0, 1, 0.2);

	float visibility = texture(ShadowSampler, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) );
//	float visibility = 1;

	fragColour = vec4(visibility * MaterialDiffuseColour * LightColour, 1);
//	fragColour = vec4(0, 1, 0, 1);

}
