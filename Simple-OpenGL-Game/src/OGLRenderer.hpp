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

class World;

class OGLRenderer
{
public:
	typedef std::unique_ptr<sf::RenderWindow> WindowUPtr;

	enum VertexArrays
	{
		CubeSimpleVAO,
//		CubeTexVAO,
		CubeDirLightVAO,
		CubePntLightVAO,
		EarthSimpleVAO,
//		EarthTexVAO,
		EarthDirLightVAO,
		EarthPntLightVAO,
		LightVAO,
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
		LightColBuf,
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
		Tex2DProgram,
		DirLightProgram,
		PntLightProgram,
		NumPrograms
	};

private:
	GLuint						mVAO[NumVAO];
	GLuint						mVBO[NumVBO];
	GLuint						mEBO[NumEBO];

	GLuint 						mTex[NumTex];
	GLenum 						mTexTargets[NumTex];

	GLuint						mShadingProgramID[NumPrograms];

//	glm::mat4					mVPMatrix;

	glm::mat4 					mVMatrix;
	glm::mat4 					mPMatrix;

	glm::vec3 					*mDirLightSource;
	std::vector<glm::vec3> 	*mPntLightSources;

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

	void 						draw(glm::mat4 MVP
									, glm::mat4 MV
									, GLuint VAO
									, GLuint EBO
									, GLuint numElements
									, GLuint texBuffer
									, GLenum texTarget
									, Shader *shdrInfo
									, Programs progType);

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

//	glm::mat4 				getVPMatrix()
//								{ return mVPMatrix; }

	glm::mat4 				getPMatrix()
								{ return mPMatrix; }

	glm::mat4 				getVMatrix()
								{ return mVMatrix; }

	GLuint					getShaderProgramID(int program)
								{ return mShadingProgramID[program]; }

	Shader*					getShaderInfo(int program)
								{ return mShaderInfo.at(program).get(); }

	unsigned int 			getShaderCount()
								{ return mShaderInfo.size(); }

	// Setters
	void 						setDirLightSource(glm::vec3 *newDirLightSource)
								{ mDirLightSource = newDirLightSource; }

	void 						setPntLightSources(std::vector<glm::vec3> *newPntLightSources)
								{ mPntLightSources = newPntLightSources; }
};

#endif // OGLRENDERER_HPP
