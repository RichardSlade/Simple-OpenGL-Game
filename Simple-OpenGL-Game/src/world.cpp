#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "World.hpp"
#include "OGLRenderer.hpp"
//#include "Texture2DShader.hpp"
//#include "DirLightShader.hpp"
#include "SceneNode.hpp"
#include "ComputeMatricesFromInput.hpp"

World::World(OGLRenderer &renderer)
: mRenderer(renderer)
, mSceneGraph(Node::NodeType::Root)
{
	// Load meshes
	loadObjects();

	// Send mesh data to renderer for storage
	if(!mRenderer.loadWorldData(this))
	{
		std::cerr << "Error: mRenderer.loadWorldData()!" << std::endl;
		exit(1);
	}

	// Build world using allocated mesh data
   buildWorld();

   mRenderer.setDirLightSource(&mDirLightSource);
   mRenderer.setPntLightSources(&mPntLightSources);
}

void World::loadObjects()
{
	try
	{
		mObjectMeshes.push_back(OBJMesh::OBJMeshPtr(new OBJMesh("Cube"
																					,"models/cubeWoodBox.obj"
																					, Mesh::ShadingType::Phong)));

		mObjectMeshes.push_back(OBJMesh::OBJMeshPtr(new OBJMesh("Earth"
																, "models/earth.obj"
																, Mesh::ShadingType::Phong)));
	}
	catch(std::exception &e)
	{
		std::cerr << "Error loading objects!\n" << e.what() << std::endl;
		exit(1);
	}
}

void World::buildWorld()
{
	int shaderCount = mRenderer.getShaderCount();

	for(int i = 0; i < shaderCount - 2; i++)
	{
		std::unique_ptr<ShaderNode> shaderNode(new ShaderNode(Node::NodeType::ShdrNode
																			 , mRenderer
																			 , mRenderer.getShaderInfo(i)
																			 , static_cast<OGLRenderer::Programs>(i)
																			 , mRenderer.getShaderProgramID(i)));

		mSceneGraphShaderNodes.push_back(dynamic_cast<ShaderNode*>(shaderNode.get()));
		mSceneGraph.attachChild(std::move(shaderNode));
	}

	glm::vec3 pos(-20, 10, 10);
	glm::vec3 lightScale(2.f, 2.f, 2.f);

	// Create directional light source
	mDirLightSource = glm::vec3(pos);

	createObject(LightObj
					, ShaderNodeTypes::SimpleShaderNode
					, pos
					, lightScale);

	lightScale = glm::vec3(0.25, 0.25, 0.25);

//	mLightProjMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	mLightProjMatrix = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.0f);
	mLightViewMatrix = glm::lookAt(mDirLightSource, -mDirLightSource, glm::vec3(0,1,0));
	mRenderer.setLightVPMatrices(mLightViewMatrix
											, mLightProjMatrix);

	// Create point light sources
//	for(int i = 0; i < 1; i++)
//	{
//		pos = glm::vec3(i, 40, 10);
//
//		mPntLightSources.push_back(pos);
//
//		createObject(LightObj
//						, ShaderNodeTypes::SimpleShaderNode
//						, pos
//						, lightScale);
//	}

//	createObject(SpherePntLightObj
//					, ShaderNodeTypes::PntLightShaderNode
//					, glm::vec3(0, 1, 0));

	// Floor
//	createObject(FloorObj
////					, ShaderNodeTypes::SimpleShaderNode
//					, ShaderNodeTypes::ShadowShaderNode
//					, glm::vec3(0));

	// Create objects
	createObject(CubeDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(4, 1, 0));

	createObject(SphereDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(-4, 1, 0));
}

void World::createObject(ObjectTypes objType
								, ShaderNodeTypes objShdr
								, glm::vec3 objPos
								, glm::vec3 objScale)
{
	std::unique_ptr<SceneNode> obj = nullptr;

	// Initialise object with requested attributes
	switch(objType)
	{
		case CubeDirLightObj:
		case CubePntLightObj:
		{
			OGLRenderer::VertexArrays vao;

			if(objType == CubeDirLightObj)
				vao = OGLRenderer::VertexArrays::CubeDirLightVAO;
			else
				vao = OGLRenderer::VertexArrays::CubePntLightVAO;

			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(vao)
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::CubeShadowVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
																		, mObjectMeshes.at(MeshType::Cube)->elementNum
																		, mRenderer.getTex(OGLRenderer::Textures::WoodBoxTex)
																		, mRenderer.getTexTarget(OGLRenderer::Textures::WoodBoxTex)));

			break;
		}
		case SphereDirLightObj:
		case SpherePntLightObj:
		{
			OGLRenderer::VertexArrays vao;

			if(objType == SphereDirLightObj)
				vao = OGLRenderer::VertexArrays::EarthDirLightVAO;
			else
				vao = OGLRenderer::VertexArrays::EarthPntLightVAO;

			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(vao)
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthShadowVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::SphereEBO)
																		, mObjectMeshes.at(MeshType::Earth)->elementNum
																		, mRenderer.getTex(OGLRenderer::Textures::BlueMarbleTex)
																		, mRenderer.getTexTarget(OGLRenderer::Textures::BlueMarbleTex)));

			break;
		}
		case LightObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::LightVAO)
																		, 0
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::SphereEBO)
																		, mObjectMeshes.at(MeshType::Earth)->elementNum));

			break;
		}
		case FloorObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::PlaneVAO)
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::PlaneShadowVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::PlaneEBO)
																		, 6));

			break;
		}
		default: break;
	}

	// Store in scene graph
	mSceneGraphShaderNodes.at(objShdr)->attachChild(std::move(obj));
}

void World::drawWorld()
{
//	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
//	glm::mat4 depthProjectionMatrix = glm::frustrum<float>(-1.0f,1.0f,-10,10,-10,20);
//	glm::mat4 depthProjectionMatrix = glm::mat4(1);
//	glm::mat4 depthProjectionMatrix = glm::;
//	glm::mat4 depthViewMatrix = glm::lookAt(getViewDirection(), -mDirLightSource, glm::vec3(0,1,0));
//	glm::mat4 depthViewMatrix = glm::lookAt(-mDirLightSource, glm::vec3(0,0,0), glm::vec3(0,1,0));

//	glm::mat4 depthModelMatrix = glm::mat4(1);
//	glm::mat4 depthModelMatrix = glm::translate(mDirLightSource);
//	glm::mat4 depthMVP = getProjectionMatrix() * depthViewMatrix * depthModelMatrix;
//	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
//	glm::mat4 depthMVP = glm::mat4(1);

//   mRenderer.setupShadowDepthShader();

	enum isDepthPass {Normal, Depth};

	// Shadow map pass
//	mRenderer.setupDepthPass(mLightViewMatrix
//								, mLightProjMatrix);

//	mRenderer.clearContext(isDepthPass::Depth);
//	mRenderer.setupShader(mRenderer.getShaderProgramID(OGLRenderer::Programs::DepthProgram));

//	mSceneGraph.drawShadowMap();

//	mRenderer.clearContext(isDepthPass::Normal);
//	mRenderer.drawShadowMap();

	// Now regular rendering pass
//	mRenderer.clearContext(isDepthPass::Normal);

//	mSceneGraph.draw();

//	mSceneGraph.draw(mRenderer.getPlayerViewMatrix()
//							, mRenderer.getPlayerProjMatrix());
}
