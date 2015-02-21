#ifndef SIMPLESHADER_HPP
#define SIMPLESHADER_HPP

#include "Shader.hpp"

struct SimpleShader : public Shader
{
public:
						SimpleShader()
						: Shader()
						{

						};

	virtual 			~SimpleShader(){};
};

#endif // SIMPLESHADER_HPP
