#version 330

uniform sampler2DShadow ShadowSampler;
//uniform sampler2D DepthTexture;
uniform sampler2D Tex2DSampler;
uniform vec3 LightPosition;

layout(location = 0) out vec4 FragColour;

in VS_FS_INTERFACE
{
	vec4 shadowCoord;
//	vec4 colour;
	vec2 uv;
	vec3 worldCoord;
	vec3 eyeCoord;
	vec3 normal;
} Fragment;

//vec2 poissonDisk[16] = vec2[](
//   vec2( -0.94201624, -0.39906216 ),
//   vec2( 0.94558609, -0.76890725 ),
//   vec2( -0.094184101, -0.92938870 ),
//   vec2( 0.34495938, 0.29387760 ),
//   vec2( -0.91588581, 0.45771432 ),
//   vec2( -0.81544232, -0.87912464 ),
//   vec2( -0.38277543, 0.27676845 ),
//   vec2( 0.97484398, 0.75648379 ),
//   vec2( 0.44323325, -0.97511554 ),
//   vec2( 0.53742981, -0.47373420 ),
//   vec2( -0.26496911, -0.41893023 ),
//   vec2( 0.79197514, 0.19090188 ),
//   vec2( -0.24188840, 0.99706507 ),
//   vec2( -0.81409955, 0.91437590 ),
//   vec2( 0.19984126, 0.78641367 ),
//   vec2( 0.14383161, -0.14100790 )
//);
//
//// Returns a random number based on a vec3 and an int.
//float random(vec3 seed, int i){
//	vec4 seed4 = vec4(seed,i);
//	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
//	return fract(sin(dot_product) * 43758.5453);
//}

void main()
{
	vec3 ambient = vec3(0.1);
	vec3 lightColour = vec3(0.9);

	float shininess = 20;
	float strength = 0.5;

	vec3 N = Fragment.normal;
   vec3 L = normalize(LightPosition);
   vec3 R = reflect(-L, N);
   vec3 E = normalize(Fragment.eyeCoord);

   float NdotL = dot(N, L);
   float EdotR = dot(E, R);

   float thetaDiffuse = max(NdotL, 0.0);
   float thetaSpecular = max(EdotR, 0.0);

	if(thetaDiffuse == 0.0)
		thetaSpecular = 0.0;
	else
		thetaSpecular = pow(thetaSpecular, shininess);

	vec3 scatteredLight = lightColour + thetaDiffuse;
	vec3 reflectedLight = lightColour * thetaSpecular * strength;

	float bias = 0.0001 * tan(acos(clamp(NdotL, 0, 1))); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, 0, 0.001);

	vec4 tempShadowCoord = Fragment.shadowCoord;
	tempShadowCoord.z -= bias;

	float f = textureProj(ShadowSampler, tempShadowCoord);

////	int index = 0;
////
////   for(int i = 0; i < 16; i++)
////	{
//////		int index = int(16.0 * random(floor(Fragment.worldCoord.xyz * 1000.0), i)) % 16;
////		f -= 0.2 * (1.0 - texture(DepthSampler, vec3(Fragment.shadowCoord.xy + poissonDisk[i] / 700.0,  (Fragment.shadowCoord.z - bias) / Fragment.shadowCoord.w)));
////	}

	vec4 texColour = texture2D(Tex2DSampler, Fragment.uv);
//	vec4 texColour = vec4(1, 0, 1, 1);

////	vec3 rgb = min(Fragment.colour.rgb * scatteredLight + reflectedLight, vec3(1.0));
	vec3 rgb = min(texColour.rgb * scatteredLight + reflectedLight, vec3(1.0));

   FragColour = vec4(f * rgb, texColour.a);
//   FragColour = vec4(rgb, texColour.a);
}
