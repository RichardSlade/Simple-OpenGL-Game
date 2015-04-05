#ifndef TEXTURE2DSHADER_HPP
#define TEXTURE2DSHADER_HPP

#include "Shader.hpp"

struct Texture2DShader : public Shader
{
public:
	GLint 			UV;
	GLint				sampler;

						Texture2DShader()
						: Shader()
						{};

	virtual 			~Texture2DShader(){};
};

#endif // TEXTURE2DSHADER_HPP

