#version 330 core

struct PointLightSource
{
	bool 			isEnabled;
	int 			type;

	vec3			ambient;
	vec3			colour;
	vec3 			position;
    vec3 			halfVector;

	float			constantAttenuation;
	float			linearAttenuation;
	float 			quadraticAttenuation;
};

uniform PointLightSource LightSource;

uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;

in vec4 Position;
in vec2 UV;
in vec3 Normal;

uniform sampler2D Tex2DSampler;

out vec4 FragColour;

void main()
{
	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);

	vec3 halfVector;
	vec3 lightDirection = LightSource.position;

	lightDirection = lightDirection - vec3(Position);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;

	float attenuation = 1.0 /
				  (LightSource.constantAttenuation
					+ LightSource.linearAttenuation * lightDistance
					+ LightSource.quadraticAttenuation * lightDistance
					(* lightDistance);



	halfVector = normalize(lightDirection + EyeDirection);

	float diffuse = max(0.0, dot(Normal, lightDirection));
	float specular = max(0.0, dot(Normal, halfVector));

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * Strength;

	scatteredLight += LightSource.ambient * attenuation
						+ LightSource.colour * diffuse * attenuation;

	reflectedLight += LightSource.colour * specular * attenuation;


	vec4 texColour = texture2D(Tex2DSampler, UV);
	vec3 rgb = min(texColour.rgb * vec3(0.7, 0.7, 0.7), vec3(1.0));
	FragColour = vec4(rgb, texColour.a);
}



