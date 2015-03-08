#ifndef SHADOWSHADER_HPP
#define SHADOWSHADER_HPP

#include "LightingShader.hpp"

struct ShadowShader : public LightingShader
{
public:
	// Uniforms
   GLuint 				shadowSampler;
	GLuint 				depthMVP;
};

#endif // SHADOWSHADER_HPP
