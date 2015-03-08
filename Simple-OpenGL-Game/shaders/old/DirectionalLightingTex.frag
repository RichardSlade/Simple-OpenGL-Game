#version 330 core

uniform sampler2D Tex2DSampler;
//uniform vec3 LightPosition;
uniform vec3 LightDirection;
uniform vec3 HalfVector;
//uniform float Shininess;
//uniform float Strength;

in vec2 UV;
in vec3 Normal;
//in vec3 HalfVector;
//in vec3 LightDirection;

out vec4 FragColour;

void main()
{
	vec4 texColour = texture2D(Tex2DSampler, UV);
	vec3 Ambient = vec3(0.1, 0.1, 0.1);
	vec3 LightColour = vec3(0.9, 0.9, 0.9);

	float Shininess = 20;
	float Strength = 1;

	// Compute cosine of the directions, using dot products,
	// to see how much light would be reflected
	float thetaDiffuse = max(0.0, dot(Normal, LightDirection));
	float thetaSpecular = max(0.0, dot(Normal, HalfVector));

	// Surfaces facing away from light (negative dot products)
	// won't be lit by the directional light
	if(thetaDiffuse == 0.0)
		thetaSpecular = 0.0;
	else
		thetaSpecular = pow(thetaSpecular, Shininess); // Sharpen the highlight

	// Don't modulate the underlying colour with reflected light
	// only with scattered light
	vec3 scatteredLight = Ambient + LightColour * thetaDiffuse;
	vec3 reflectedLight = LightColour * thetaSpecular * Strength;

	vec3 rgb = min(texColour.rgb * scatteredLight + reflectedLight, vec3(1.0));
	FragColour = vec4(rgb, texColour.a);
}

