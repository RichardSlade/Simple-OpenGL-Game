#include <iostream>

#include "OGLRenderer.hpp"
#include "World.hpp"
#include "ComputeMatricesFromInput.hpp"
#include "SimpleShader.hpp"
#include "Texture2DShader.hpp"
#include "DirectionalLightingShaderTex.hpp"

GLuint SamplerID;

OGLRenderer::~OGLRenderer()
{
	cleanUp();
}

bool OGLRenderer::init(sf::RenderWindow *window)
{
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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
	loadTextures();

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
	mShadingProgramID[Texture2DProgram] = LoadShaders(shader_info_two);
	mShaderInfo.push_back(Shader::ShaderUPtr(new Texture2DShader()));

	ShaderInfo shader_info_three[] =
    {
        { GL_VERTEX_SHADER, "shaders/DirectionalLightingTex.vert" },
        { GL_FRAGMENT_SHADER, "shaders/DirectionalLightingTex.frag" },
        { GL_NONE, NULL }
    };

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[DirectionLightingProgram] = LoadShaders(shader_info_three);
	mShaderInfo.push_back(Shader::ShaderUPtr(new DirectionalLightingShaderTex()));

	allocProgramUniforms();
}

bool OGLRenderer::loadTextures()
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

	// Cube Simple Shader VAO
	glBindVertexArray(mVAO[CubeSimpleVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Colour
								, 4
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Colour);

	// Cube Texture2D shader VAO
	glBindVertexArray(mVAO[CubeTexVAO]);

	Texture2DShader *tex2DShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Texture2DProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
	glVertexAttribPointer(tex2DShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(tex2DShader->Position);
//	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeTexBuf]);
	glVertexAttribPointer(tex2DShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(tex2DShader->UV);
//	glEnableVertexAttribArray(1);

	// Load Earth mesh data
	OBJMesh *earthMesh = world->getObject(World::MeshType::Earth);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO[EarthEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, earthMesh->elementNum * sizeof(GLushort), &earthMesh->elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->vertNum * sizeof(glm::vec4), &earthMesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->normalNum * sizeof(glm::vec3), &earthMesh->normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
	glBufferData(GL_ARRAY_BUFFER, earthMesh->texCordNum * sizeof(glm::vec2), &earthMesh->texCords[0], GL_STATIC_DRAW);

	// Earth Simple Shader VAO
	glBindVertexArray(mVAO[EarthSimpleVAO]);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Position);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(mShaderInfo.at(SimpleProgram)->Colour
								, 4
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(mShaderInfo.at(SimpleProgram)->Colour);

	// Earth Texture2D shader VAO
	glBindVertexArray(mVAO[EarthTexVAO]);

//	Texture2DShader *tex2DShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Texture2DProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(tex2DShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(tex2DShader->Position);
//	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
	glVertexAttribPointer(tex2DShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(tex2DShader->UV);
//	glEnableVertexAttribArray(1);

	// Earth Directional shader VAO
	glBindVertexArray(mVAO[EarthDirLightVAO]);

	DirectionalLightingShaderTex *dirLightShader = dynamic_cast<DirectionalLightingShaderTex*>(mShaderInfo.at(DirectionLightingProgram).get());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
	glVertexAttribPointer(dirLightShader->Position
								, 4
								, GL_FLOAT
								, GL_FALSE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(dirLightShader->Position);
//	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
	glVertexAttribPointer(dirLightShader->UV
								, 2
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthNormBuf]);
	glVertexAttribPointer(dirLightShader->Normal
								, 3
								, GL_FLOAT
								, GL_TRUE
								, 0
								, (void*)0);

	glEnableVertexAttribArray(dirLightShader->Normal);
//	glEnableVertexAttribArray(1);

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

	Texture2DShader *tex2DShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Texture2DProgram).get());

	// Texture shader uniforms
	tex2DShader->MVP = glGetUniformLocation(mShadingProgramID[Texture2DProgram], "MVP");
	tex2DShader->Sampler = glGetUniformLocation(mShadingProgramID[Texture2DProgram], "Tex2DSampler");
//	glGetUniformLocation(mShadingProgramID[Texture2DProgram], "Tex2DSampler");

	// Texture shader variables
	tex2DShader->Position = glGetAttribLocation(mShadingProgramID[Texture2DProgram], "vPosition");
	tex2DShader->UV = glGetAttribLocation(mShadingProgramID[Texture2DProgram], "vUV");

	DirectionalLightingShaderTex *dirTexShader = dynamic_cast<DirectionalLightingShaderTex*>(mShaderInfo.at(DirectionLightingProgram).get());

	// Directional lighting shader uniforms
	dirTexShader->MVP = glGetUniformLocation(mShadingProgramID[DirectionLightingProgram], "MVP");
	dirTexShader->MV = glGetUniformLocation(mShadingProgramID[DirectionLightingProgram], "MV");
	dirTexShader->LightPosition = glGetUniformLocation(mShadingProgramID[DirectionLightingProgram], "LightPosition");
	dirTexShader->Sampler = glGetUniformLocation(mShadingProgramID[DirectionLightingProgram], "Tex2DSampler");

	// Directional lighting shader variables
	dirTexShader->Position = glGetAttribLocation(mShadingProgramID[DirectionLightingProgram], "vPosition");
	dirTexShader->UV = glGetAttribLocation(mShadingProgramID[DirectionLightingProgram], "vUV");
	dirTexShader->Normal = glGetAttribLocation(mShadingProgramID[DirectionLightingProgram], "vNormal");

//	mShaders.at(Texture2DProgram) = text2DShader;

//	std::cout <<  "Pos: " << mShaders.at(SimpleProgram).Position
//				<< "\nCol: " << mShaders.at(SimpleProgram).Colour
//				<<"\nMVP: " << mShaders.at(SimpleProgram).MVP << std::endl;

//	// Vertex shader
//	ShadingProgramID[SimpleProgram] = glGetUniformLocation( LightMultiProgramID, "MVPMatrix");
//	LightMultiProgramMVMatrixID = glGetUniformLocation( LightMultiProgramID, "MVMatrix");
//	LightMultiProgramNormMatrixID = glGetUniformLocation( LightMultiProgramID, "NormalMatrix");
//
//	// Fragment shader
//	LightMultiProgramEyeDirectionID = glGetUniformLocation(LightMultiProgramID, "EyeDirection");
//	LightMultiProgram2DSamplerID = glGetUniformLocation(LightMultiProgramID, "Tex2DSampler");
//
//	// Vertex shader
//	GLint uniLoc = glGetUniformLocation(LightMultiProgramID, "Shininess");
//	glUniform1f(uniLoc, Shininess);
//
//	uniLoc = glGetUniformLocation(LightMultiProgramID, "Strength");
//	glUniform1f(uniLoc, Strength);
}

void OGLRenderer::clearContext()
{
	glClearColor(0.3f, 0.3f, 0.5f, 1.f);

	// Clear the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderer::setupShader(int shaderID)
{
	glUseProgram(shaderID);
}

void OGLRenderer::draw(GLuint VAO
							, GLuint EBO
							, GLuint numElements
							, GLuint texID
							, GLenum texTarget
							, GLint samplerID
							, GLint MVPID
							, glm::mat4 MVP)
{
	glBindVertexArray(VAO);

	// If node uses texture i.e sampler uniform has been assigned
	if(samplerID > 0)
	{
		glUniform1i(samplerID, texID - 1);
		glActiveTexture(texTarget);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, NULL);
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

void OGLRenderer::calcVPMatrix()
{
	glm::mat4 P = getProjectionMatrix();
	glm::mat4 V = getViewMatrix();
	mVPMatrix = P * V;
}
