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

	loadPrograms();
	loadTextures(window->getSize());

	return true;
}

void OGLRenderer::loadPrograms()
{
	ShaderInfo shader_info_one[] =
    {
        { GL_VERTEX_SHADER, "shaders/Simple.vert" },
        { GL_FRAGMENT_SHADER, "shaders/Simple.frag" },
        { GL_NONE, NULL }
    };

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[SimpleProgram] = LoadShaders(shader_info_one);
	mShaderInfo.push_back(Shader::ShaderUPtr(new SimpleShader()));

//	std::cout << "ShaderID: " << mShadingProgramID[SimpleProgram] << std::endl;

	ShaderInfo shader_info_two[] =
    {
        { GL_VERTEX_SHADER, "shaders/Texturing2D.vert" },
        { GL_FRAGMENT_SHADER, "shaders/Texturing2D.frag" },
        { GL_NONE, NULL }
    };

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[Tex2DProgram] = LoadShaders(shader_info_two);
	mShaderInfo.push_back(Shader::ShaderUPtr(new Texture2DShader()));

	ShaderInfo shader_info_three[] =
    {
        { GL_VERTEX_SHADER, "shaders/DirectionalLightingTex.vert" },
        { GL_FRAGMENT_SHADER, "shaders/DirectionalLightingTex.frag" },
        { GL_NONE, NULL }
    };

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[DirLightProgram] = LoadShaders(shader_info_three);
	mShaderInfo.push_back(Shader::ShaderUPtr(new LightingShader()));

	ShaderInfo shader_info_four[] =
    {
        { GL_VERTEX_SHADER, "shaders/PointLighting.vert" },
        { GL_FRAGMENT_SHADER, "shaders/PointLighting.frag" },
        { GL_NONE, NULL }
    };

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[PntLightProgram] = LoadShaders(shader_info_four);
	mShaderInfo.push_back(Shader::ShaderUPtr(new LightingShader()));

	ShaderInfo shader_info_five[] =
	{
        { GL_VERTEX_SHADER, "shaders/DepthRTT.vert" },
        { GL_FRAGMENT_SHADER, "shaders/DepthRTT.frag" },
        { GL_NONE, NULL }
	};

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[DepthProgram] = LoadShaders(shader_info_five);
	mShaderInfo.push_back(Shader::ShaderUPtr(new ShadowShader()));

	ShaderInfo shader_info_six[] =
	{
        { GL_VERTEX_SHADER, "shaders/ShadowMapping_SimpleVersion.vert" },
        { GL_FRAGMENT_SHADER, "shaders/ShadowMapping_SimpleVersion.frag" },
        { GL_NONE, NULL }
	};

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[ShadowProgram] = LoadShaders(shader_info_six);
	mShaderInfo.push_back(Shader::ShaderUPtr(new ShadowShader()));

	ShaderInfo shader_info_seven[] =
	{
        { GL_VERTEX_SHADER, "shaders/ShadowMap.vert" },
        { GL_FRAGMENT_SHADER, "shaders/ShadowMap.frag" },
        { GL_NONE, NULL }
	};

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[DepthTexProgram] = LoadShaders(shader_info_seven);
	mShaderInfo.push_back(Shader::ShaderUPtr(new ShadowShader()));

	allocProgramUniforms();
}

bool OGLRenderer::loadTextures(sf::Vector2u windowSize)
{
	glGenTextures(NumTex, &mTex[0]);

	mTexTargets[WoodBoxTex] = GL_TEXTURE0;
//	glActiveTexture(mTexTargets[WoodBoxTex]);

	sf::Image image;
	if (!image.loadFromFile("media/WoodBox.png"))
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
//	glActiveTexture(mTexTargets[BlueMarbleTex]);

	if (!image.loadFromFile("media/BlueMarble.png"))
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

	mTexTargets[DepthTex] = GL_TEXTURE2;
//	glActiveTexture(mTexTargets[DepthTex]);

	glBindTexture(GL_TEXTURE_2D, mTex[DepthTex]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32
					 , static_cast<float>(windowSize.x) * 2.f, static_cast<float>(windowSize.y) * 2.f
					 , 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
						GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

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

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeTexBuf]);
	glBufferData(GL_ARRAY_BUFFER, cubeMesh->texCordNum * sizeof(glm::vec2), &cubeMesh->texCords[0], GL_STATIC_DRAW);

	// Cube Directional light
	glBindVertexArray(mVAO[CubeDirLightVAO]);

	LightingShader *lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(lightShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeTexBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeNormBuf]);
	glVertexAttribPointer(lightShader->Normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Normal);

	// Cube Point light
	glBindVertexArray(mVAO[CubeDirLightVAO]);

	lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(PntLightProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(lightShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeTexBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeNormBuf]);
	glVertexAttribPointer(lightShader->Normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Normal);

	glBindVertexArray(mVAO[CubeShadowVAO]);

	ShadowShader *shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(shadowShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(shadowShader->Position);

//	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeTexBuf]);
//	glVertexAttribPointer(shadowShader->UV
//								, 4
//								, GL_FLOAT
//								, GL_FALSE
//								, 0
//								, (void*)0);
//
//	glEnableVertexAttribArray(shadowShader->UV);

	// Load Earth mesh data
	OBJMesh *earthMesh = world->getObject(World::MeshType::Earth);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[SphereEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, earthMesh->elementNum * sizeof(GLushort), &earthMesh->elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->vertNum * sizeof(glm::vec4), &earthMesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->normalNum * sizeof(glm::vec3), &earthMesh->normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->texCordNum * sizeof(glm::vec2), &earthMesh->texCords[0], GL_STATIC_DRAW);

	// Earth Directional shader VAO
	glBindVertexArray(mVAO[EarthDirLightVAO]);

	lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(lightShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glVertexAttribPointer(lightShader->Normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Normal);

	// Data for Pnt light earth
	glBindVertexArray(mVAO[EarthPntLightVAO]);

	lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(PntLightProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(lightShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
	glVertexAttribPointer(lightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->UV);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glVertexAttribPointer(lightShader->Normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(lightShader->Normal);

	glBindVertexArray(mVAO[EarthShadowVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(shadowShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(shadowShader->Position);

	// Data for lights
	std::vector<glm::vec4> lightColours;

	for(unsigned int i = 0; i < earthMesh->vertNum; i++)
		lightColours.push_back(glm::vec4(1, 1, 0.5, 1));

   glBindBuffer(GL_ARRAY_BUFFER, mVBO[LightColBuf]);
   glBufferData(GL_ARRAY_BUFFER, lightColours.size() * sizeof(glm::vec4), &lightColours[0], GL_STATIC_DRAW);

	glBindVertexArray(mVAO[LightVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[LightColBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Colour
								, 4
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Colour);

	// Data for plane used as floor
	std::vector<glm::vec4> planeColours;

	// Colour green for grass
	for(unsigned int i = 0; i < earthMesh->vertNum; i++)
		planeColours.push_back(glm::vec4(0, 1, 0.25, 1));

   glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneColBuf]);
   glBufferData(GL_ARRAY_BUFFER, planeColours.size() * sizeof(glm::vec4), &planeColours[0], GL_STATIC_DRAW);

	const GLfloat planeVerts[] =
	{
		-50, 0,  50, 1,
		 50, 0,  50, 1,
		 50, 0, -50, 1,
		-50, 0, -50, 1,
	};

	const GLushort planeEle[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts), planeVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[PlaneEBO]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeEle), planeEle, GL_STATIC_DRAW);

	glBindVertexArray(mVAO[PlaneVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneColBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Colour
								, 4
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Colour);

	glBindVertexArray(mVAO[PlaneShadowVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[PlaneVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(DepthProgram)->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(DepthProgram)->Position);

	glBindVertexArray(mVAO[ShadowMapVAO]);

	GLfloat quadVerts[] =
	{
		0.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,

		0.f, 1.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		1.f, 1.f, 0.f, 1.f,
	};

	GLfloat quadUV[] =
	{
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,

		0.f, 1.f,
		1.f, 0.f,
		1.f, 1.f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(GLfloat), &quadVerts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadUVBuf]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(GLfloat), &quadUV[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[QuadVertBuf]);
	glVertexAttribPointer(0
								, 4
								, GL_FLOAT
								, GL_TRUE
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

	return true;
}

void OGLRenderer::allocProgramUniforms()
{
	// Simple Shader uniforms
	mShaderInfo.at(SimpleProgram)->MVP = glGetUniformLocation(mShadingProgramID[SimpleProgram], "MVP");

	// Simple Shader variables
	mShaderInfo.at(SimpleProgram)->Position = glGetAttribLocation(mShadingProgramID[SimpleProgram], "vPosition");
	mShaderInfo.at(SimpleProgram)->Colour = glGetAttribLocation(mShadingProgramID[SimpleProgram], "vColour");

	Texture2DShader *tex2DShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Tex2DProgram).get());

	// Texture shader uniforms
	tex2DShader->MVP = glGetUniformLocation(mShadingProgramID[Tex2DProgram], "MVP");
	tex2DShader->Sampler = glGetUniformLocation(mShadingProgramID[Tex2DProgram], "Tex2DSampler");

	// Texture shader variables
	tex2DShader->Position = glGetAttribLocation(mShadingProgramID[Tex2DProgram], "vPosition");
	tex2DShader->UV = glGetAttribLocation(mShadingProgramID[Tex2DProgram], "vUV");

	LightingShader *lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());

	// Directional lighting shader uniforms
	lightShader->MVP = glGetUniformLocation(mShadingProgramID[DirLightProgram], "MVP");
//	dirLightShader->MV = glGetUniformLocation(mShadingProgramID[DirLightProgram], "MV");
	lightShader->NormalMatrix = glGetUniformLocation(mShadingProgramID[DirLightProgram], "NormalMatrix");
//	dirLightShader->LightPosition = glGetUniformLocation(mShadingProgramID[DirLightProgram], "LightPosition");
	lightShader->LightDirection = glGetUniformLocation(mShadingProgramID[DirLightProgram], "LightDirection");
//	dirLightShader->ViewDirection = glGetUniformLocation(mShadingProgramID[DirLightProgram], "ViewDirection");
	lightShader->HalfVector = glGetUniformLocation(mShadingProgramID[DirLightProgram], "HalfVector");
	lightShader->Sampler = glGetUniformLocation(mShadingProgramID[DirLightProgram], "Tex2DSampler");

	// Directional lighting shader variables
	lightShader->Position = glGetAttribLocation(mShadingProgramID[DirLightProgram], "vPosition");
	lightShader->UV = glGetAttribLocation(mShadingProgramID[DirLightProgram], "vUV");
	lightShader->Normal = glGetAttribLocation(mShadingProgramID[DirLightProgram], "vNormal");

//	lightShader = nullptr;
	lightShader = dynamic_cast<LightingShader*>(mShaderInfo.at(PntLightProgram).get());

	// Point lighting shader uniforms
	lightShader->MVP = glGetUniformLocation(mShadingProgramID[PntLightProgram], "MVP");
	lightShader->MV = glGetUniformLocation(mShadingProgramID[PntLightProgram], "MV");
	lightShader->NormalMatrix = glGetUniformLocation(mShadingProgramID[PntLightProgram], "NormalMatrix");
	lightShader->LightPosition = glGetUniformLocation(mShadingProgramID[PntLightProgram], "LightPosition");
	lightShader->EyeDirection = glGetUniformLocation(mShadingProgramID[PntLightProgram], "EyeDirection");
	lightShader->Sampler = glGetUniformLocation(mShadingProgramID[PntLightProgram], "Tex2DSampler");

	// Pntectional lighting shader variables
	lightShader->Position = glGetAttribLocation(mShadingProgramID[PntLightProgram], "vPosition");
	lightShader->UV = glGetAttribLocation(mShadingProgramID[PntLightProgram], "vUV");
	lightShader->Normal = glGetAttribLocation(mShadingProgramID[PntLightProgram], "vNormal");

	// Depth programs
	ShadowShader *shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());

	shadowShader->DepthMVP = glGetUniformLocation(mShadingProgramID[DepthProgram], "DepthMVP");
	shadowShader->Position = glGetAttribLocation(mShadingProgramID[DepthProgram], "vPosition");

	shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(ShadowProgram).get());

	shadowShader->MVP = glGetUniformLocation(mShadingProgramID[ShadowProgram], "MVP");
	shadowShader->DepthMVP = glGetUniformLocation(mShadingProgramID[ShadowProgram], "DepthBiasMVP");
//	shadowShader->Sampler = glGetUniformLocation(mShadingProgramID[ShadowProgram], "Tex2DSample");
	shadowShader->ShadowSampler = glGetUniformLocation(mShadingProgramID[ShadowProgram], "ShadowSampler");

	shadowShader->Position = glGetAttribLocation(mShadingProgramID[ShadowProgram], "vPosition");
//	shadowShader->UV = glGetAttribLocation(mShadingProgramID[ShadowProgram], "vUV");
//	shadowShader->Normal = glGetAttribLocation(mShadingProgramID[ShadowProgram], "vNormal");

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
		glDeleteProgram(mShadingProgramID[i]);
}

unsigned int OGLRenderer::texTargetToUInt(GLenum texTarget)
{
	switch(texTarget)
	{
		case GL_TEXTURE0: return 0;
		case GL_TEXTURE1: return 1;
		case GL_TEXTURE2: return 2;
		default: return 999;
	}
}

void OGLRenderer::clearContext(bool isShadowPass)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(isShadowPass)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);

	glClearColor(0.3f, 0.3f, 0.5f, 1.f);
   glEnable(GL_CULL_FACE);

   if(isShadowPass)
	{
		glCullFace(GL_FRONT | GL_BACK);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void OGLRenderer::setupDepthPass(glm::mat4 lightView
											, glm::mat4 lightProj)
{
	mLightViewMatrix = lightView;
	mLightProjMatrix = lightProj;
}

void OGLRenderer::setupShader(int shaderID)
{

//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(shaderID);

//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);

//	glClearColor(0.3f, 0.3f, 0.5f, 1.f);

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderer::drawShadowMap()
{
	glUseProgram(mShadingProgramID[DepthTexProgram]);
	glBindVertexArray(mVAO[ShadowMapVAO]);

//	GLuint tex2DsamplerID = glGetUniformLocation(mShadingProgramID[DepthTexProgram], "Tex2DSampler");

//	glUniform1i(1, 2);
//	glActiveTexture(mTexTargets[DepthTex]);
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, mTex[DepthTex]);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OGLRenderer::drawShadow(glm::mat4 depthMVP
									 , GLuint VAO
									 , GLuint EBO
									 , GLuint numElements
)
{
	glBindVertexArray(VAO);

	ShadowShader* shadowShader = dynamic_cast<ShadowShader*>(mShaderInfo.at(DepthProgram).get());
	glUniformMatrix4fv(shadowShader->DepthMVP, 1, GL_FALSE, &depthMVP[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, NULL);
}

void OGLRenderer::draw(//glm::mat4 MVP
							//, glm::mat4 MV
							 glm::mat4 M
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

	glm::mat4 MVP = mPlayerProjMatrix * mPlayerViewMatrix * M;
	glm::mat4 MV = mPlayerViewMatrix * M;

	switch(progType)
	{
		case SimpleProgram:
		{
			SimpleShader *shdr = dynamic_cast<SimpleShader*>(mShaderInfo.at(SimpleProgram).get());
			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);
			break;
		}
		case Tex2DProgram:
		{
			Texture2DShader *shdr = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Tex2DProgram).get());
			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);

			glUniform1i(shdr->Sampler, texTargetToUInt(texTarget));
			glActiveTexture(texTarget);
			glBindTexture(GL_TEXTURE_2D, texID);

			break;
		}
		case DirLightProgram:
		{
			LightingShader *shdr = dynamic_cast<LightingShader*>(mShaderInfo.at(DirLightProgram).get());
			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);

			glm::mat3 normMatrix(MV);
			glUniformMatrix3fv(shdr->NormalMatrix, 1, GL_FALSE, &normMatrix[0][0]);

			glm::vec3 lightDirection(glm::normalize(MV * glm::vec4(-*mDirLightSource, 0)));
			glUniform3fv(shdr->LightDirection, 1, glm::value_ptr(lightDirection));

			glm::vec3 eyeDir = glm::vec3(MV * glm::vec4(getViewDirection(), 0));

			glm::vec3 halfVec = glm::normalize(eyeDir + lightDirection);
			glUniform3fv(shdr->HalfVector, 1, &halfVec[0]);

			glUniform1i(shdr->Sampler, texTargetToUInt(texTarget));
			glActiveTexture(texTarget);
			glBindTexture(GL_TEXTURE_2D, texID);

			break;
		}
		case PntLightProgram:
		{
			LightingShader *shdr = dynamic_cast<LightingShader*>(mShaderInfo.at(PntLightProgram).get());
			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(shdr->MV, 1, GL_FALSE, &MV[0][0]);

			glm::mat3 normMatrix(MV);
			glUniformMatrix3fv(shdr->NormalMatrix, 1, GL_FALSE, &normMatrix[0][0]);

			glUniform3fv(shdr->LightPosition, 1, glm::value_ptr(MV * glm::vec4(-mPntLightSources->at(0), 1)));
			glUniform3fv(shdr->EyeDirection, 1, glm::value_ptr(MV * glm::vec4(getViewDirection(), 0)));

			glUniform1i(shdr->Sampler, texTargetToUInt(texTarget));
			glActiveTexture(texTarget);
			glBindTexture(GL_TEXTURE_2D, texID);

			break;
		}
		case ShadowProgram:
		{
			ShadowShader *shdr = dynamic_cast<ShadowShader*>(mShaderInfo.at(ShadowProgram).get());
			glUniformMatrix4fv(shdr->MVP, 1, GL_FALSE, &MVP[0][0]);

			glm::mat4 biasMatrix(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);

//			glm::mat4 biasDepthMVP = glm::mat4(1);
			glm::mat4 biasDepthMVP = mLightProjMatrix * mLightViewMatrix * M;
			biasDepthMVP = biasMatrix * biasDepthMVP;
//			glm::mat4 biasDepthMVP = biasMatrix * mDepthMVP;
         glUniformMatrix4fv(shdr->DepthMVP, 1, GL_FALSE, &biasDepthMVP[0][0]);

//         glm::mat3 normMatrix(MV);
//         glUniformMatrix3fv(shdr->NormalMatrix, 1, GL_FALSE, &normMatrix[0][0]);

//			glUniform1i(shdr->Sampler, texID - 1);
//			glActiveTexture(texTarget);
//			glBindTexture(GL_TEXTURE_2D, texID);

			glUniform1i(shdr->ShadowSampler, texTargetToUInt(mTexTargets[DepthTex]));
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

	return std::move(newWindow);
}

void OGLRenderer::setPlayerVPMatrices()
{
	mPlayerProjMatrix = getProjectionMatrix();
	mPlayerViewMatrix = getViewMatrix();
//	mVPMatrix = mPMatrix * mVMatrix;
}

void OGLRenderer::setLightVPMatrices(glm::mat4 view
												, glm::mat4 proj)
{
	mLightProjMatrix = proj;
	mLightViewMatrix = view;
//	mVPMatrix = mPMatrix * mVMatrix;
}
