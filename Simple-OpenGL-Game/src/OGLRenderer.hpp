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
//		CubeSimpleVAO,
//		CubeTexVAO,
		CubeDirLightVAO,
		CubePntLightVAO,
		CubeShadowVAO,
//		EarthSimpleVAO,
//		EarthTexVAO,
		EarthDirLightVAO,
		EarthPntLightVAO,
		EarthShadowVAO,
		LightVAO,
		PlaneVAO,
		PlaneShadowVAO,
		ShadowMapVAO,
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
		PlaneVertBuf,
		PlaneColBuf,
		QuadVertBuf,
		QuadColBuf,
		QuadUVBuf,
		NumVBO
	};

	enum ElementArrayBuffers
	{
		CubeEBO,
		SphereEBO,
		PlaneEBO,
		NumEBO
	};

	enum Textures
	{
		WoodBoxTex,
		BlueMarbleTex,
		DepthTex,
		NumTex
	};

	enum Programs
	{
		SimpleProgram,
		Tex2DProgram,
		DirLightProgram,
		PntLightProgram,
		ShadowProgram,
		DepthProgram,
		DepthTexProgram,
		NumPrograms
	};

private:
	GLuint						mVAO[NumVAO];
	GLuint						mVBO[NumVBO];
	GLuint						mEBO[NumEBO];

	GLuint 						mTex[NumTex];
	GLenum 						mTexTargets[NumTex];

	GLuint						mFBO;

	GLuint						mShadingProgramID[NumPrograms];

	glm::mat4 					mPlayerViewMatrix;
	glm::mat4 					mPlayerProjMatrix;

	glm::mat4 					mLightViewMatrix;
	glm::mat4 					mLightProjMatrix;
//	glm::mat4 					mDepthMVP;

	glm::vec3 					*mDirLightSource;
	std::vector<glm::vec3> 	*mPntLightSources;

	std::vector<Shader::ShaderUPtr> mShaderInfo;

	void 						loadPrograms();
	bool						loadTextures(sf::Vector2u windowSize);
	void						allocProgramUniforms();

	bool 						checkFramebuffer(GLenum status);

	void 						cleanUp();

	unsigned int  			texTargetToUInt(GLenum texTarget);

public:
								OGLRenderer(){};
								~OGLRenderer();

	WindowUPtr				createWindow();
	bool 						init(sf::RenderWindow *window);
	bool 						loadWorldData(World *world);

	void 						clearContext(bool isShadowPass);

	void 						setupDepthPass(glm::mat4 lightView
											, glm::mat4 lightProj);

	void 						setupShader(int shaderID);

	void 						drawShadowMap();

	void						drawShadow(glm::mat4 MVP
											 , GLuint VAO
											 , GLuint EBO
											 , GLuint numElements);

	void 						draw(glm::mat4 M
									, GLuint VAO
									, GLuint EBO
									, GLuint numElements
									, GLuint texBuffer
									, GLenum texTarget
									, Shader *shdrInfo
									, Programs progType);

	void 						setPlayerVPMatrices();
	void 						setLightVPMatrices(glm::mat4 lightView
														, glm::mat4 lightProj);

	// Getters
	GLuint 					getVAO(VertexArrays vao)
								{ return mVAO[vao]; }

	GLuint 					getEBO(ElementArrayBuffers ebo)
								{ return mEBO[ebo]; }

	GLuint 					getTex(Textures tex)
								{ return mTex[tex]; }

	GLenum 					getTexTarget(Textures tex)
								{ return mTexTargets[tex]; }

	glm::mat4 				getPlayerProjMatrix()
								{ return mPlayerProjMatrix; }

	glm::mat4 				getPlayerViewMatrix()
								{ return mPlayerViewMatrix; }

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
