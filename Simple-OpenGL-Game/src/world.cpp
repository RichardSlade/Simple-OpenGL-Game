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

	glm::vec3 pos(16.f, 40.f, 20.f);
	glm::vec3 lightScale(2.f, 2.f, 2.f);

	// Create directional light source
//	mDirLightSource = glm::vec3(pos);
	mDirLightSource = LightSource(pos, -pos);

	createObject(LightObj
					, ShaderNodeTypes::SimpleShaderNode
					, pos
					, lightScale);

	mRenderer.setDirLightSource(&mDirLightSource);

	// Floor
	createObject(FloorDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(0));

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
//		case CubePntLightObj:
		{
			OGLRenderer::VertexArrays vao;

//			if(objType == CubeDirLightObj)
				vao = OGLRenderer::VertexArrays::CubeDirLightVAO;
//			else
//				vao = OGLRenderer::VertexArrays::CubePntLightVAO;

			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(vao)
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::CubeDepthVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
																		, mObjectMeshes.at(MeshType::Cube)->elementNum
																		, mRenderer.getTex(OGLRenderer::Textures::WoodBoxTex)
																		, mRenderer.getTexTarget(OGLRenderer::Textures::WoodBoxTex)));

			break;
		}
		case SphereDirLightObj:
//		case SpherePntLightObj:
		{
			OGLRenderer::VertexArrays vao;

//			if(objType == SphereDirLightObj)
				vao = OGLRenderer::VertexArrays::EarthDirLightVAO;
//			else
//				vao = OGLRenderer::VertexArrays::EarthPntLightVAO;

			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(vao)
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthDepthVAO)
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
		case FloorDirLightObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::PlaneDirLightVAO)
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::PlaneDepthVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::PlaneEBO)
																		, 6
																		, mRenderer.getTex(OGLRenderer::Textures::StoneTex)
																		, mRenderer.getTexTarget(OGLRenderer::Textures::StoneTex)));

			break;
		}
		default: break;
	}

	// Store in scene graph
	mSceneGraphShaderNodes.at(objShdr)->attachChild(std::move(obj));
}

void World::drawWorld()
{
	// Shadow map pass
	mRenderer.setupDepthPass();
	mSceneGraph.depthPass();
	mRenderer.finishDepthPass();

//	mRenderer.drawShadowMap();

	// Regular rendering pass
	mRenderer.clearContext();
	mSceneGraph.draw();
}
