#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "OGLRenderer.hpp"
#include "World.hpp"
#include "ComputeMatricesFromInput.hpp"
#include "SimpleShader.hpp"
#include "Texture2DShader.hpp"
#include "LightingShader.hpp"

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

//	ShaderInfo shader_info_four[] =
//    {
//        { GL_VERTEX_SHADER, "shaders/DirectionalLightingTex.vert" },
//        { GL_FRAGMENT_SHADER, "shaders/DirectionalLightingTex.frag" },
//        { GL_NONE, NULL }
//    };

	// Create and compile our GLSL program from the shaders
	mShadingProgramID[PntLightProgram] = LoadShaders(shader_info_four);
	mShaderInfo.push_back(Shader::ShaderUPtr(new LightingShader()));

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
//	glBindVertexArray(mVAO[CubeTexVAO]);
//
//	Texture2DShader *tex2DShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Tex2DProgram).get());
//
//	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeVertBuf]);
//	glVertexAttribPointer(tex2DShader->Position
//								, 4
//								, GL_FLOAT
//								, GL_FALSE
//								, 0
//								, (void*)0);
//
//	glEnableVertexAttribArray(tex2DShader->Position);
////	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mVBO[CubeTexBuf]);
//	glVertexAttribPointer(tex2DShader->UV
//								, 2
//								, GL_FLOAT
//								, GL_TRUE
//								, 0
//								, (void*)0);
//
//	glEnableVertexAttribArray(tex2DShader->UV);
//	glEnableVertexAttribArray(1);

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
//	glBindVertexArray(mVAO[EarthTexVAO]);

//	Texture2DShader *tex2DShader = dynamic_cast<Texture2DShader*>(mShaderInfo.at(Tex2DProgram).get());

//	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthVertBuf]);
//	glVertexAttribPointer(tex2DShader->Position
//								, 4
//								, GL_FLOAT
//								, GL_FALSE
//								, 0
//								, (void*)0);
//
//	glEnableVertexAttribArray(tex2DShader->Position);
////	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, mVBO[EarthTexBuf]);
//	glVertexAttribPointer(tex2DShader->UV
//								, 2
//								, GL_FLOAT
//								, GL_TRUE
//								, 0
//								, (void*)0);
//
//	glEnableVertexAttribArray(tex2DShader->UV);
//	glEnableVertexAttribArray(1);

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

	// Data for lights
	std::vector<glm::vec4> lightColours;

	for(int i = 0; i < earthMesh->vertNum; i++)
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

void OGLRenderer::draw(glm::mat4 MVP
							, glm::mat4 MV
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

			glUniform1i(shdr->Sampler, texID - 1);
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

			glUniform1i(shdr->Sampler, texID - 1);
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

			glUniform1i(shdr->Sampler, texID - 1);
			glActiveTexture(texTarget);
			glBindTexture(GL_TEXTURE_2D, texID);

			break;
		}
		default: break;
	}

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
	mPMatrix = getProjectionMatrix();
	mVMatrix = getViewMatrix();
//	mVPMatrix = mPMatrix * mVMatrix;
}
