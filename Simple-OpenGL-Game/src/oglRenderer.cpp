#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "OGLRenderer.hpp"
#include "World.hpp"
#include "ComputeMatricesFromInput.hpp"
#include "SimpleShader.hpp"
#include "Texture2DShader.hpp"
#include "LightingShader.hpp"
#include "ShadowShader.hpp"

GLuint SamplerID;

OGLRenderer::~OGLRenderer()
{
	cleanUp();
}

bool OGLRenderer::init(sf::RenderWindow *window)
{
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Disable lighting
	glDisable(GL_LIGHTING);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window->getSize().x, window->getSize().y);

		// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	if(!loadPrograms())
		return false;

	loadTextures(window->getSize());

	return true;
}

bool OGLRenderer::loadPrograms()
{
	int pID;

	// Colour
	ShaderInfo shader_info_one[] =
    {
        { GL_VERTEX_SHADER, "shaders/Colour.vert" },
        { GL_FRAGMENT_SHADER, "shaders/Colour.frag" },
        { GL_NONE, NULL }
    };

	// Create and compile our GLSL program from the shaders
	pID = LoadShaders(shader_info_one);

	if(pID == 0)
	{
		std::cerr << "Error: OGLRenderer.loadPrograms(): Simple Shader" << std::endl;
		return false;
	}

	mProgramID[ColourProgram] = pID;
	mShaderInfo.push_back(Shader::ShaderUPtr(new SimpleShader()));

	// Dir Lighting
	ShaderInfo shader_info_two[] =
	{
		{ GL_VERTEX_SHADER, "shaders/DirLighting.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/DirLighting.frag" },
		{ GL_NONE, NULL }
	};

	pID = LoadShaders(shader_info_two);

	if(pID == 0)
	{
		std::cerr << "Error: OGLRenderer.loadPrograms(): DirLighting Shader" << std::endl;
		return false;
	}

	// Create and compile our GLSL program from the shaders
	mProgramID[DirLightProgram] = pID;
	mShaderInfo.push_back(Shader::ShaderUPtr(new LightingShader()));

	// Depth
	ShaderInfo shader_info_three[] =
	{
        { GL_VERTEX_SHADER, "shaders/DepthRTT.vert" },
        { GL_FRAGMENT_SHADER, "shaders/DepthRTT.frag" },
        { GL_NONE, NULL }
	};

	pID = LoadShaders(shader_info_three);

	if(pID == 0)
	{
		std::cerr << "Error: OGLRenderer.loadPrograms(): Depth Shader" << std::endl;
		return false;
	}

	// Create and compile our GLSL program from the shaders
	mProgramID[DepthProgram] = pID;
	mShaderInfo.push_back(Shader::ShaderUPtr(new ShadowShader()));

	//Tex 2D
	ShaderInfo shader_info_four[] =
	{
        { GL_VERTEX_SHADER, "shaders/Texturing2D.vert" },
        { GL_FRAGMENT_SHADER, "shaders/Texturing2D.frag" },
        { GL_NONE, NULL }
	};

	pID = LoadShaders(shader_info_four);

	if(pID == 0)
	{
		std::cerr << "Error: OGLRenderer.loadPrograms(): Depth Shader" << std::endl;
		return false;
	}

	// Create and compile our GLSL program from the shaders
	mProgramID[Tex2DProgram] = pID;
	mShaderInfo.push_back(Shader::ShaderUPtr(new Texture2DShader()));

	allocProgramUniforms();

	return true;
}

bool OGLRenderer::loadTextures(sf::Vector2u windowSize)
{
	glGenTextures(NumTex, &mTex[0]);

	mTexTargets[WoodBoxTex] = GL_TEXTURE0;
//	glActiveTexture(mTexTargets[WoodBoxTex]);

	sf::Image image;
	if (!image.loadFromFile("media/woodBox.png"))
		return EXIT_FAILURE;

	glBindTexture(GL_TEXTURE_2D, mTex[WoodBoxTex]);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, image.getSize().x, image.getSize().y);
	glTexSubImage2D(GL_TEXTURE_2D
					, 0
					, 0, 0
					, image.getSize().x, image.getSize().y
					, GL_RGBA, GL_UNSIGNED_BYTE
					, image.getPixelsPtr());

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);

	mTexTargets[BlueMarbleTex] = GL_TEXTURE1;
	if (!image.loadFromFile("media/blueMarble.png"))
		return EXIT_FAILURE;

	glBindTexture(GL_TEXTURE_2D, mTex[BlueMarbleTex]);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, image.getSize().x, image.getSize().y);
	glTexSubImage2D(GL_TEXTURE_2D
					, 0
					, 0, 0
					, image.getSize().x, image.getSize().y
					, GL_RGBA, GL_UNSIGNED_BYTE
					, image.getPixelsPtr());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Stone
	mTexTargets[StoneTex] = GL_TEXTURE2;

	if (!image.loadFromFile("media/stone.png"))
		return EXIT_FAILURE;

	glBindTexture(GL_TEXTURE_2D, mTex[StoneTex]);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA8, image.getSize().x, image.getSize().y);
	glTexSubImage2D(GL_TEXTURE_2D
					, 0
					, 0, 0
					, image.getSize().x, image.getSize().y
					, GL_RGBA, GL_UNSIGNED_BYTE
					, image.getPixelsPtr());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Depth
	mTexTargets[DepthTex] = GL_TEXTURE3;
	glBindTexture(GL_TEXTURE_2D, mTex[DepthTex]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32
					 , static_cast<float>(windowSize.x) * 2.f, static_cast<float>(windowSize.y) * 2.f
					 , 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT
								, mTex[DepthTex], 0);

	if(!checkFramebuffer(glCheckFramebufferStatus(GL_FRAMEBUFFER)))
	{
		std::cout << "Error initialising framebuffer" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool OGLRenderer::loadWorldData(World *world)
{
	// Load cube data
	OBJMesh *cubeMesh = world->getObject(World::MeshType::Cube);

	glGenVertexArrays(NumVAO, &mVAO[0]);
	glGenBuffers(NumEBO, &mEBO[0]);
	glGenBuffers(NumVBO, &mVBO[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[CubeEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeMesh->elementNum * sizeof(GLushort), &cubeMesh->elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, cubeMesh->vertNum * sizeof(glm::vec4), &cubeMesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeNormBuf]);
	glBufferData(GL_ARRAY_BUFFER, cubeMesh->normalNum * sizeof(glm::vec3), &cubeMesh->normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeUVBuf]);
	glBufferData(GL_ARRAY_BUFFER, cubeMesh->texCordNum * sizeof(glm::vec2), &cubeMesh->texCords[0], GL_STATIC_DRAW);

	// Load Earth mesh data
	OBJMesh *earthMesh = world->getObject(World::MeshType::Earth);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[SphereEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, earthMesh->elementNum * sizeof(GLushort), &earthMesh->elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->vertNum * sizeof(glm::vec4), &earthMesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->normalNum * sizeof(glm::vec3), &earthMesh->normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthUVBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->texCordNum * sizeof(glm::vec2), &earthMesh->texCords[0], GL_STATIC_DRAW);

		// Data for lights
	std::vector<glm::vec4> lightColours;

	for(unsigned int i = 0; i < earthMesh->vertNum; i++)
		lightColours.push_back(glm::vec4(1, 1, 0.5, 1));

   glBindBuffer(GL_ARRAY_BUFFER, mVBO[LightColBuf]);
   glBufferData(GL_ARRAY_BUFFER, lightColours.size() * sizeof(glm::vec4), &lightColours[0], GL_STATIC_DRAW);

   // Data for plane used as floor
	std::vector<glm::vec4> planeColours;

	// Colour green for grass
	for(unsigned int i = 0; i < earthMesh->vertNum; i++)
		planeColours.push_back(glm::vec4(0, 1, 0.25, 1));

	std::vector<glm::vec3> planeNorms;

	for(int i = 0; i < 6; i++)
		planeNorms.push_back(glm::vec3(0, 1, 0));

	const GLfloat planeVerts[] =
	{
		-50, 0,  50, 1,
		 50, 0,  50, 1,
		 50, 0, -50, 1,
		-50, 0, -50, 1,
	};

	const GLfloat planeUV[] =
	{
		0.f, 0.f,
		50.f, 0.f,
		50.f, 50.f,
		0.f, 50.f,
	};

	const GLushort planeEle[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts), &planeVerts[0], GL_STATIC_DRAW);

//   glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneColBuf]);
//   glBufferData(GL_ARRAY_BUFFER, planeColours.size() * sizeof(glm::vec4), &planeColours[0], GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneUVBuf]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(planeUV), &planeUV[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneNormBuf]);
   glBufferData(GL_ARRAY_BUFFER, planeNorms.size() * sizeof(glm::vec3), &planeNorms[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[PlaneEBO]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeEle), &planeEle[0], GL_STATIC_DRAW);

   const GLfloat quadVerts[] =
	{
		-1.f, -1.f, 0.f, 1.f,
		 1.f, -1.f, 0.f, 1.f,
		 1.f,  1.f, 0.f, 1.f,

		-1.f, -1.f, 0.f, 1.f,
		 1.f,  1.f, 0.f, 1.f,
		-1.f,  1.f, 0.f, 1.f,
	};

	const GLfloat quadUV[] =
	{
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,

		0.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadUVBuf]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(quadUV), &quadUV[0], GL_STATIC_DRAW);

	setupVAO();

	return true;
}

void OGLRenderer::setupVAO()
{
	// Cube Directional light
	glBindVertexArray(mVAO[CubeDirLightVAO]);

	LightingShader *lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(lightShader->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeUVBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeNormBuf]);
	glVertexAttribPointer(lightShader->normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->normal);

	// Cube depth
	glBindVertexArray(mVAO[CubeDepthVAO]);

	ShadowShader *shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(shadowShader->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(shadowShader->position);

	// Earth Directional shader VAO
	glBindVertexArray(mVAO[EarthDirLightVAO]);

	lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(lightShader->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthUVBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glVertexAttribPointer(lightShader->normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->normal);

	// Earth depth
	glBindVertexArray(mVAO[EarthDepthVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(shadowShader->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(shadowShader->position);

	// Light VAO
	glBindVertexArray(mVAO[LightVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(ColourProgram)->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(ColourProgram)->position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[LightColBuf]);
	glVertexAttribPointer(mShaderInfo.at(ColourProgram)->colour
								, 4
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(ColourProgram)->colour);

	// Plane VAO
	glBindVertexArray(mVAO[PlaneDirLightVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneVertBuf]);
	glVertexAttribPointer(lightShader->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneUVBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneNormBuf]);
	glVertexAttribPointer(lightShader->normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->normal);

	// Plane depth
	glBindVertexArray(mVAO[PlaneDepthVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(DepthProgram)->position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(DepthProgram)->position);

	// Quad
//	Texture2DShader *texShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Tex2DProgram).get());

	glBindVertexArray(mVAO[QuadVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadVertBuf]);
	glVertexAttribPointer(0
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadUVBuf]);
	glVertexAttribPointer(1
								, 2
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void OGLRenderer::allocProgramUniforms()
{
	// Simple Shader uniforms
	mShaderInfo.at(ColourProgram)->MVP = glGetUniformLocation(mProgramID[ColourProgram], "MVP");

	// Simple Shader variables
	mShaderInfo.at(ColourProgram)->position = glGetAttribLocation(mProgramID[ColourProgram], "vPosition");
	mShaderInfo.at(ColourProgram)->colour = glGetAttribLocation(mProgramID[ColourProgram], "vColour");

	// Directional lighting shader uniforms
	LightingShader *lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

	lightShader->M = glGetUniformLocation(mProgramID[DirLightProgram], "M");
	lightShader->V = glGetUniformLocation(mProgramID[DirLightProgram], "V");
	lightShader->P = glGetUniformLocation(mProgramID[DirLightProgram], "P");
	lightShader->shadowMatrix = glGetUniformLocation(mProgramID[DirLightProgram], "ShadowMatrix");
	lightShader->lightPosition = glGetUniformLocation(mProgramID[DirLightProgram], "LightPosition");
	lightShader->shadowSampler = glGetUniformLocation(mProgramID[DirLightProgram], "ShadowSampler");
	lightShader->sampler = glGetUniformLocation(mProgramID[DirLightProgram], "Tex2DSampler");

	// Directional lighting shader variables
	lightShader->position = glGetAttribLocation(mProgramID[DirLightProgram], "vPosition");
	lightShader->UV = glGetAttribLocation(mProgramID[DirLightProgram], "vUV");
	lightShader->normal = glGetAttribLocation(mProgramID[DirLightProgram], "vNormal");

	// Depth program
	ShadowShader *shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());

	shadowShader->depthMVP = glGetUniformLocation(mProgramID[DepthProgram], "DepthMVP");
	shadowShader->position = glGetAttribLocation(mProgramID[DepthProgram], "vPosition");
}

bool OGLRenderer::checkFramebuffer(GLenum status)
{
	bool complete = false;

	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
		{
			std::cout << "Complete" << std::endl;
			complete = true;

			break;
		}
		case GL_FRAMEBUFFER_UNDEFINED:
		{
			std::cout << "undefined" << std::endl;

			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			std::cout << "incomplete attach" << std::endl;

			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			std::cout << "missing attach" << std::endl;

			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			std::cout << "incomplete draw" << std::endl;

			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{
			std::cout << "incomplete read" << std::endl;

			break;
		}
		case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			std::cout << "unsupported" << std::endl;

			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{
			std::cout << "incomplete multi" << std::endl;

			break;
		}
		default: std::cout << "Default" << std::endl; break;
	}

	return complete;
}

void OGLRenderer::cleanUp()
{
	glDeleteTextures(NumTex, &mTex[0]);
	glDeleteVertexArrays(NumVAO, &mVAO[0]);
	glDeleteBuffers(NumVBO, &mVBO[0]);
	glDeleteBuffers(NumEBO, &mEBO[0]);

	for(int i = 0; i < NumPrograms; i++)
		glDeleteProgram(mProgramID[i]);
}

unsigned int OGLRenderer::texTargetToUInt(GLenum texTarget)
{
	switch(texTarget)
	{
		case GL_TEXTURE0: return 0;
		case GL_TEXTURE1: return 1;
		case GL_TEXTURE2: return 2;
		case GL_TEXTURE3: return 3;
		default: return 999;
	}
}

void OGLRenderer::clearContext()
{
	glClearColor(0.3f, 0.3f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderer::setupDepthPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glViewport(0, 0, mWindow->getSize().x * 2, mWindow->getSize().y * 2);
	glCullFace(GL_FRONT);

}

void OGLRenderer::finishDepthPass()
{
	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

//	glClearColor(0.3f, 0.3f, 0.5f, 1.f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0,  mWindow->getSize().x, mWindow->getSize().y);
	glCullFace(GL_BACK);
}

void OGLRenderer::setupShader(int shaderID)
{
	glUseProgram(shaderID);
}

void OGLRenderer::drawShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(mProgramID[Tex2DProgram]);

	glClearColor(0.2f, 0.5f, 0.2f, 1.f);
	glViewport(0, 0, mWindow->getSize().x, mWindow->getSize().y);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint tex2DsamplerID = glGetUniformLocation(mProgramID[Tex2DProgram], "Tex2DSampler");

	glUniform1i(tex2DsamplerID, texTargetToUInt(mTexTargets[DepthTex]));
	glActiveTexture(mTexTargets[DepthTex]);
	glBindTexture(GL_TEXTURE_2D, mTex[DepthTex]);

	glBindVertexArray(mVAO[QuadVAO]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OGLRenderer::drawDepth(glm::mat4 M
									 , GLuint VAO
									 , GLuint EBO
									 , GLuint numElements
)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
//	glClear(GL_DEPTH_BUFFER_BIT);

//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glViewport(0, 0, mWindow->getSize().x * 2, mWindow->getSize().y * 2);
//	glCullFace(GL_FRONT);

	glUseProgram(mProgramID[DepthProgram]);

	glm::mat4 depthMVP = mDirLightSource->P * mDirLightSource->V * M;
//	glm::mat4 depthMVP = getProjectionMatrix() * getViewMatrix() * M;

	ShadowShader* shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());
	glUniformMatrix4fv(shadowShader->depthMVP, 1, GL_FALSE, &depthMVP[0][0]);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLRenderer::draw(glm::mat4 M
							, GLuint VAO
							, GLuint EBO
							, GLuint numElements
							, GLuint texID
							, GLenum texTarget
							, Shader *shdrInfo
							, Programs progType)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glm::mat4 P = getProjectionMatrix();
	glm::mat4 V = getViewMatrix();

	switch(progType)
	{
		case ColourProgram:
		{
			SimpleShader *shdr = dynamic_cast<SimpleShader*>(mShaderInfo.at(ColourProgram).get());

			glm::mat4 MVP = P * V * M;
			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);
			break;
		}
		case DirLightProgram:
		{
			LightingShader *shdr = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

			glUniformMatrix4fv(shdr->P, 1, GL_FALSE, &P[0][0]);
			glUniformMatrix4fv(shdr->V, 1, GL_FALSE, &V[0][0]);
			glUniformMatrix4fv(shdr->M, 1, GL_FALSE, &M[0][0]);

			glm::mat4 biasMatrix(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);

			glm::mat4 depthMVP = mDirLightSource->P * mDirLightSource->V * M;
			depthMVP = biasMatrix * depthMVP;
			glUniformMatrix4fv(shdr->shadowMatrix, 1, GL_FALSE, &depthMVP[0][0]);

			glUniform1i(shdr->shadowSampler, texTargetToUInt(mTexTargets[DepthTex]));
			glActiveTexture(mTexTargets[DepthTex]);
			glBindTexture(GL_TEXTURE_2D, mTex[DepthTex]);

			glm::vec3 lightDirection(glm::normalize(glm::mat3(V) * -mDirLightSource->position));
			glUniform3fv(shdr->lightPosition, 1, &lightDirection[0]);

			glUniform1i(shdr->sampler, texTargetToUInt(texTarget));
			glActiveTexture(texTarget);
			glBindTexture(GL_TEXTURE_2D, texID);

			break;
		}
		case DepthProgram:
		{
			ShadowShader *shdr = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());
//			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);

			glm::mat4 biasMatrix(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);

//			glm::mat4 biasDepthMVP = mLightProjMatrix * mLightViewMatrix * M;
			glm::mat4 biasDepthMVP = mDirLightSource->P * mDirLightSource->V * M;
			biasDepthMVP = biasMatrix * biasDepthMVP;
         glUniformMatrix4fv(shdr->depthMVP, 1, GL_FALSE, &biasDepthMVP[0][0]);

			glUniform1i(shdr->shadowSampler, texTargetToUInt(mTexTargets[DepthTex]));
			glActiveTexture(mTexTargets[DepthTex]);
			glBindTexture(GL_TEXTURE_2D, mTex[DepthTex]);

			break;
		}
		default: break;
	}

	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, NULL);
}

OGLRenderer::WindowUPtr OGLRenderer::createWindow()
{
	// Request a 32-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
    contextSettings.stencilBits = 32;
    contextSettings.antialiasingLevel = 3;
    contextSettings.minorVersion = 3;
    contextSettings.majorVersion = 4;

    // Create the main window
    WindowUPtr newWindow = WindowUPtr(new sf::RenderWindow(sf::VideoMode(800, 600)
								, "SFML graphics with OpenGL"
								, sf::Style::Default, contextSettings));

    newWindow->setVerticalSyncEnabled(true);
    newWindow->setMouseCursorVisible(false);

    newWindow->setActive();

    mWindow = newWindow.get();

	return std::move(newWindow);
}

