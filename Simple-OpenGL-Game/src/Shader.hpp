#ifndef SHADER_HPP
#define SHADER_HPP

#include <memory>

struct Shader
{
public:
	typedef std::unique_ptr<Shader> ShaderUPtr;

	// Uniforms
	GLint 				MVP;
	GLint 				MV;
	GLint					NormMat;

	// Variables
	GLint				Position;
	GLint				Colour;
	GLint				UV;

						Shader()
						: MVP(0)
						, MV(0)
						, NormMat(0)
						, Position(0)
						, Colour(0)
						, UV(0)
						{};

	virtual 			~Shader(){};
};

#endif // SHADER_HPP
