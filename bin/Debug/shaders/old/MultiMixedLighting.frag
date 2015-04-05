#version 330 core

struct LightSource
{
	bool 			isEnabled;
	int 			type;

	vec3			ambient;
	vec3			colour;
	vec3 			position;

    vec3 			halfVector;
//    vec3			coneDirection;

//	float 			spotCosCutoff;
//	float			spotExponent;
	float			constantAttenuation;
	float			linearAttenuation;
	float 			quadraticAttenuation;
};

const int MaxLights = 1;
uniform LightSource Lights[MaxLights];

uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;

in vec4 Position;
in vec2 UV;
in vec3 Normal;
in mat4 MVMatrixFrag;

uniform sampler2D Tex2DSampler;

out vec4 FragColour;

void main()
{
	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);

	for(int light = 0; light < MaxLights; ++light)
	{
		if(!Lights[light].isEnabled)
			continue;

		vec3 halfVector;
//		vec3 lightDirection = (MVMatrixFrag * vec4(Lights[light].position, 0)).xyz;
		vec3 lightDirection = Lights[light].position;
		float attenuation = 1.0;

		if(Lights[light].type > 0)
		{
			lightDirection = lightDirection - vec3(Position);
			float lightDistance = length(lightDirection);
			lightDirection = lightDirection / lightDistance;

//			attenuation = 1.0 /
//						  (Lights[light].constantAttenuation
//							+ Lights[light].linearAttenuation * lightDistance
//							+ Lights[light].quadraticAttenuation * lightDistance
//							(* lightDistance);

			attenuation = 0.1;

//			if(Lights[light].isSpot)
//			{
//				float spotCos = dot(lightDirection
//									, -Lights[light].coneDirection);
//
//				if(spotCos < Lights[light].spotCosCutoff)
//					attenuation = 0.0;
//				else
//					attenuation *= pow(spotCos
//									  , Lights[light].spotExponent);
//			}

			halfVector = normalize(lightDirection + EyeDirection);
		}
		else
		{
			halfVector = Lights[light].halfVector;
		}

		float diffuse = max(0.0, dot(Normal, lightDirection));
		float specular = max(0.0, dot(Normal, halfVector));

		if(diffuse == 0.0)
			specular = 0.0;
		else
			specular = pow(specular, Shininess) * Strength;

		scatteredLight += Lights[light].ambient * attenuation
							+ Lights[light].colour * diffuse;// * attenuation;

//		diffuse = 0.5;

//		scatteredLight += vec3(0.1, 0.1, 0.1)
//							+ vec3(0.8, 0.8, 0.7) * diffuse;

		reflectedLight += Lights[light].colour * specular;// * attenuation;
	}

	vec4 texColour = texture2D(Tex2DSampler, UV);
	vec3 rgb = min(texColour.rgb * scatteredLight + reflectedLight, vec3(1.0));

//	vec3 rgb = min(texColour.rgb * vec3(0.7, 0.7, 0.7), vec3(1.0));

	FragColour = vec4(rgb, texColour.a);
}


