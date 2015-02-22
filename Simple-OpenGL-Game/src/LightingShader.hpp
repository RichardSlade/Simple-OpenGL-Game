#ifndef LIGHTINGSHADER_HPP
#define LIGHTINGSHADER_HPP

#include "Texture2DShader.hpp"

struct LightingShader : public Texture2DShader
{
public:
	// Uniforms
	GLint 			MV;
	GLint 			NormalMatrix;
	GLint 			LightPosition;
	GLint 			LightDirection;
	GLint				EyeDirection;
	GLint				HalfVector;

	// Variables
	GLint 			Normal;

	virtual 			~LightingShader(){};
};

#endif // LIGHTINGSHADER_HPP
