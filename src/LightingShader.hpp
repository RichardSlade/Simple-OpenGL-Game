#ifndef LIGHTINGSHADER_HPP
#define LIGHTINGSHADER_HPP

#include "Texture2DShader.hpp"

struct LightingShader : public Texture2DShader
{
public:
	// Uniforms
//	GLint 			MV;
//	GLint 			NormalMatrix;

	GLint 			M, V, P;
	GLint 			lightPosition;
	GLint 			shadowMatrix;
	GLint 			shadowSampler;
//	GLint 			ShadowTexture
//	GLint 			LightDirection;
//	GLint				EyeDirection;
//	GLint				HalfVector;

	// Variables
	GLint 			normal;

	virtual 			~LightingShader(){};
};

#endif // LIGHTINGSHADER_HPP
