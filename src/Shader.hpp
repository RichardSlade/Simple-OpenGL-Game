#ifndef SHADER_HPP
#define SHADER_HPP

#include <memory>

struct Shader
{
public:
	typedef std::unique_ptr<Shader> ShaderUPtr;

	// Uniforms
	GLint 				MVP;
//	GLint 				MV;
//	GLint					NormMat;

	// Variables
	GLint				position;
	GLint				colour;
	GLint				UV;

						Shader()
						: MVP(0)
//						, MV(0)
//						, NormMat(0)
						, position(0)
						, colour(0)
						, UV(0)
						{};

	virtual 			~Shader(){};
};

#endif // SHADER_HPP
