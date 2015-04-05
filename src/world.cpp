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
: mOGLRenderer(renderer)
, mSceneGraph(Node::NodeType::Root)
, mOctTree(glm::vec3(50, 50, 50)
			 , 2)
{
	// Load meshes
	loadObjects();

	// Send mesh data to renderer for storage
	if(!mOGLRenderer.loadWorldData(this))
	{
		std::cerr << "Error: mOGLRenderer.loadWorldData()!" << std::endl;
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
	int shaderCount = mOGLRenderer.getShaderCount();

	for(int i = 0; i < shaderCount - 2; i++)
	{
		std::unique_ptr<ShaderNode> shaderNode(new ShaderNode(Node::NodeType::ShdrNode
																			 , mOGLRenderer
																			 , mOGLRenderer.getShaderInfo(i)
																			 , static_cast<OGLRenderer::Programs>(i)
																			 , mOGLRenderer.getShaderProgramID(i)));

		mSceneGraphShaderNodes.push_back(dynamic_cast<ShaderNode*>(shaderNode.get()));
		mSceneGraph.attachChild(std::move(shaderNode));
	}

	glm::vec3 playerPos = glm::vec3(0, 3, 5);

	// Create player, save ref for World and OGLRenderer and attach to scene graph
	std::unique_ptr<PlayerNode> playerNode(new PlayerNode(mOGLRenderer.getWindow()
																		  , Node::NodeType::PlayerNode
																		  , playerPos));
	mPlayer = playerNode.get();
	mOGLRenderer.setPlayerMatrices(playerNode->getPlayerInput());
	mSceneGraph.attachChild(std::move(playerNode));

	// Light position and scale
	glm::vec3 pos(50.f, 50.f, 50.f);
//	glm::vec3 pos(-getDimensions() / 2.f);
	glm::vec3 lightScale(2.f, 2.f, 2.f);

	// Create directional light source
//	mDirLightSource = glm::vec3(pos);

	// Create directional lights
	mDirLightSource = LightSource(pos, -pos);
//	mDirLightSource = LightSource(pos, glm::vec3(20, 0 ,0));

	createObject(LightObj
					, ShaderNodeTypes::SimpleShaderNode
					, pos
					, lightScale);

	mOGLRenderer.setDirLightSource(&mDirLightSource);

	// Floor
	createObject(FloorDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(0));

	// Create objects
	createObject(CubeDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
//					, glm::vec3(30, 1, 10));
//					, getDimensions() / 2.f);
					, glm::vec3(5, 1, 5));

	createObject(SphereDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(10, 1, 5));
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
																		, &mOGLRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mOGLRenderer.getVAO(vao)
																		, mOGLRenderer.getVAO(OGLRenderer::VertexArrays::CubeDepthVAO)
																		, mOGLRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
																		, mObjectMeshes.at(MeshType::Cube)->elementNum
																		, mOGLRenderer.getTex(OGLRenderer::Textures::WoodBoxTex)
																		, mOGLRenderer.getTexTarget(OGLRenderer::Textures::WoodBoxTex)));

			// Insert into oct tree
			mOctTree.insertInto(obj.get());

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
																		, &mOGLRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mOGLRenderer.getVAO(vao)
																		, mOGLRenderer.getVAO(OGLRenderer::VertexArrays::EarthDepthVAO)
																		, mOGLRenderer.getEBO(OGLRenderer::ElementArrayBuffers::SphereEBO)
																		, mObjectMeshes.at(MeshType::Earth)->elementNum
																		, mOGLRenderer.getTex(OGLRenderer::Textures::BlueMarbleTex)
																		, mOGLRenderer.getTexTarget(OGLRenderer::Textures::BlueMarbleTex)));

			// Insert into oct tree
			mOctTree.insertInto(obj.get());

			break;
		}
		case LightObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, &mOGLRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mOGLRenderer.getVAO(OGLRenderer::VertexArrays::LightVAO)
																		, 0
																		, mOGLRenderer.getEBO(OGLRenderer::ElementArrayBuffers::SphereEBO)
																		, mObjectMeshes.at(MeshType::Earth)->elementNum));

			break;
		}
		case FloorDirLightObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, &mOGLRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mOGLRenderer.getVAO(OGLRenderer::VertexArrays::PlaneDirLightVAO)
																		, mOGLRenderer.getVAO(OGLRenderer::VertexArrays::PlaneDepthVAO)
																		, mOGLRenderer.getEBO(OGLRenderer::ElementArrayBuffers::PlaneEBO)
																		, 6
																		, mOGLRenderer.getTex(OGLRenderer::Textures::StoneTex)
																		, mOGLRenderer.getTexTarget(OGLRenderer::Textures::StoneTex)));

			break;
		}
		default: break;
	}

	// Store in scene graph
	mSceneGraphShaderNodes.at(objShdr)->attachChild(std::move(obj));
}

void World::handleInput(sf::Time dt)
{
	mPlayer->handleInput(dt);
}

void World::drawWorld()
{
	// Shadow map pass
	mOGLRenderer.setupDepthPass();
	mSceneGraph.depthPass();
	mOGLRenderer.finishDepthPass();

//	mOGLRenderer.drawShadowMap();

	// Regular rendering pass
	mOGLRenderer.clearContext();
	mSceneGraph.draw();
}
