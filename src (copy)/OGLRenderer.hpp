#ifndef OGLRENDERER_HPP
#define OGLRENDERER_HPP

#include <memory>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "LoadShaders.h"
#include "Shader.hpp"
//#include "SimpleShader.hpp"

class World;

class OGLRenderer
{
public:
	typedef std::unique_ptr<sf::RenderWindow> WindowUPtr;

	enum VertexArrays
	{
		CubeSimpleVAO,
		CubeTexVAO,
		CubeDirLightVAO,
		EarthSimpleVAO,
		EarthTexVAO,
		EarthDirLightVAO,
		NumVAO
	};

	enum Buffers
	{
		CubeVertBuf,
		CubeNormBuf,
		CubeTexBuf,
		EarthVertBuf,
		EarthNormBuf,
		EarthTexBuf,
		NumVBO
	};

	enum ElementArrayBuffers
	{
		CubeEBO,
		EarthEBO,
		NumEBO
	};

	enum Textures
	{
		WoodBoxTex,
		BlueMarbleTex,
		NumTex
	};

	enum Programs
	{
		SimpleProgram,
		Texture2DProgram,
		DirectionLightingProgram,
		NumPrograms
	};

private:
	GLuint						mVAO[NumVAO];
	GLuint						mVBO[NumVBO];
	GLuint						mEBO[NumEBO];

	GLuint 						mTex[NumTex];
	GLenum 						mTexTargets[NumTex];

	GLuint						mShadingProgramID[NumPrograms];

	glm::mat4					mVPMatrix;

	std::vector<Shader::ShaderUPtr> mShaderInfo;

	void 						loadPrograms();
	bool						loadTextures();
	void						allocProgramUniforms();

	void 						cleanUp();

public:
								OGLRenderer(){};
								~OGLRenderer();

	WindowUPtr				createWindow();
	bool 						init(sf::RenderWindow *window);
	bool 						loadWorldData(World *world);

	void 						clearContext();

	void 						setupShader(int shaderID);

	void 						draw(GLuint VAO
									, GLuint EBO
									, GLuint numElements
									, GLuint texBuffer
									, GLenum texTarget
									, GLint samplerID
									, GLint MVPID
									, glm::mat4 MVP);

	void 						calcVPMatrix();

	// Getters
	GLuint 					getVAO(VertexArrays vao)
								{ return mVAO[vao]; }

	GLuint 					getEBO(ElementArrayBuffers ebo)
								{ return mEBO[ebo]; }

	GLuint 					getTex(Textures tex)
								{ return mTex[tex]; }

	GLenum 					getTexTarget(Textures tex)
								{ return mTexTargets[tex]; }

	glm::mat4 				getVPMatrix()
								{ return mVPMatrix; }

	GLuint					getShaderProgramID(int program)
								{ return mShadingProgramID[program]; }

	Shader*					getShaderInfo(int program)
								{ return mShaderInfo.at(program).get(); }

	unsigned int 			getShaderCount()
								{ return mShaderInfo.size(); }
};

#endif // OGLRENDERER_HPP
