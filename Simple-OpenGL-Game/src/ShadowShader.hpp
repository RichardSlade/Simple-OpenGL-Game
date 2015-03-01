#ifndef SHADOWSHADER_HPP
#define SHADOWSHADER_HPP

#include "LightingShader.hpp"

struct ShadowShader : public LightingShader
{
public:
	// Uniforms
   GLuint 				ShadowSampler;
	GLuint 				DepthMVP;
};

#endif // SHADOWSHADER_HPP
