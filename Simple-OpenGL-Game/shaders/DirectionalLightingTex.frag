#version 330 core

in vec2 UV;
in vec3 Normal;
in vec3 LightDirection;
in vec3 ViewDirection;
in vec3 ReflectionVector;
in vec3 HalfVector;

uniform sampler2D Tex2DSampler;

out vec4 FragColour;

void main()
{
	vec4 texColour = texture2D(Tex2DSampler, UV);

//	vec3 Ambient = vec3(0.2, 0.2, 0.2) * Colour.rgb;
	vec3 Ambient = vec3(0.1, 0.1, 0.1);
//	vec3 DiffuseColour = vec3(0.9, 0.9, 0.9) * Colour.rgb;
//	vec3 SpecularColour = vec3(0.5, 0.5, 0.5);
//	vec3 SpecularColour = DiffuseColour;
	vec3 LightColour = vec3(0.8, 0.8, 0.8);

	float Shininess = 20;
	float Strength = 1;

	// Compute cosine of the directions, using dot products,
	// to see how much light would be reflected
	float thetaDiffuse = max(0.0, dot(Normal, LightDirection));

//	vec3 Diffuse = DiffuseColour * thetaDiffuse;

//	float thetaSpecular = max(0.0, dot(Normal, ReflectionVector));
	float thetaSpecular = max(0.0, dot(Normal, HalfVector));

	// Surfaces facing away from light (negative dot products)
	// won't be lit by the directional light
	if(thetaDiffuse == 0.0)
		thetaSpecular = 0.0;
	else
		thetaSpecular = pow(thetaSpecular, Shininess); // Sharpen the highlight

//	vec3 Specular = SpecularColour * thetaSpecular * Strength;
//	vec3 Specular = vec3(0, 0, 0); //LightColour * thetaSpecular * Strength;

	// Don't modulate the underlying colour with reflected light
	// only with scattered light
	vec3 scatteredLight = Ambient + LightColour + thetaDiffuse;
	vec3 reflectedLight = LightColour * thetaSpecular * Strength;
//	vec3 rgb = min(Colour.rgb * scatteredLight + reflectedLight, vec3(1.0));
	vec3 rgb = min(texColour.rgb * scatteredLight + reflectedLight, vec3(1.0));

//	vec3 rgb = min(Ambient + Diffuse + Specular, vec3(1.0));

	FragColour = vec4(rgb, texColour.a);
//	FragColour = vec4(1, 1, 1, 1);
}

