#version 330 core

//struct PointLightSource
//{
//	bool 			isEnabled;
//	int 			type;
//
//	vec3			ambient;
//	vec3			colour;
//	vec3 			position;
//    vec3 			halfVector;
//
//	float			constantAttenuation;
//	float			linearAttenuation;
//	float 			quadraticAttenuation;
//};

//uniform PointLightSource LightSource;

uniform vec3 LightPosition;
//uniform float Shininess;
//uniform float Strength;
uniform vec3 EyeDirection;
uniform sampler2D Tex2DSampler;

in vec4 Position;
in vec2 UV;
in vec3 Normal;

out vec4 FragColour;

void main()
{
	vec3 ambient = vec3(0.1, 0.1, 0.1);
	vec3 lightColour = vec3(0.7, 0.7, 0.7);

	float shininess = 20;
	float strength = 2;

	float constantAttenuation = 1;
	float linearAttenuation = 0;
	float quadraticAttenuation = 0;

	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);

	vec3 lightDirection = LightPosition;

	lightDirection = (lightDirection - vec3(Position));
	float lightDistance = length(lightDirection);
	lightDirection = (lightDirection / lightDistance);

	float attenuation = 1.0 /
				  (constantAttenuation
					+ linearAttenuation * lightDistance
					+ quadraticAttenuation * lightDistance * lightDistance);

//	float attenuation = 2.0;

	vec3 halfVector = normalize(lightDirection + EyeDirection);

	float diffuse = max(0.0, dot(Normal, lightDirection));
	float specular = max(0.0, dot(Normal, halfVector));

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, shininess) * strength;

	scatteredLight += ambient * attenuation
						+ lightColour * diffuse * attenuation;

	reflectedLight += lightColour * specular * attenuation;

//	reflectedLight = vec3(0);

	vec4 texColour = texture2D(Tex2DSampler, UV);
	vec3 rgb = min(texColour.rgb * scatteredLight + reflectedLight, vec3(1.0));
	FragColour = vec4(rgb, texColour.a);

//	FragColour = vec4(1, 1, 0, 1);
//	FragColour = texColour;
}



