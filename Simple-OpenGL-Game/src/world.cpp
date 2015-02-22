#include <iostream>

#include "World.hpp"
#include "OGLRenderer.hpp"
//#include "Texture2DShader.hpp"
//#include "DirLightShader.hpp"
#include "SceneNode.hpp"

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

	for(int i = 0; i < shaderCount; i++)
	{
		std::unique_ptr<ShaderNode> shaderNode(new ShaderNode(Node::NodeType::ShdrNode
																			 , mRenderer
																			 , mRenderer.getShaderInfo(i)
																			 , static_cast<OGLRenderer::Programs>(i)
																			 , mRenderer.getShaderProgramID(i)));

		mSceneGraphShaderNodes.push_back(dynamic_cast<ShaderNode*>(shaderNode.get()));
		mSceneGraph.attachChild(std::move(shaderNode));
	}

	glm::vec3 pos(-10, 20, 10);
	glm::vec3 lightScale(2.f, 2.f, 2.f);

	// Create directional light source
	mDirLightSource = glm::vec3(pos);

	createObject(LightObj
					, ShaderNodeTypes::SimpleShaderNode
					, pos
					, lightScale);

	lightScale = glm::vec3(0.25, 0.25, 0.25);

	// Create point light sources
	for(int i = 0; i < 1; i++)
	{
		pos = glm::vec3(i, 40, 10);

		mPntLightSources.push_back(pos);

		createObject(LightObj
						, ShaderNodeTypes::SimpleShaderNode
						, pos
						, lightScale);
	}

	// Create objects
//	createObject(CubeColObj
//					, ShaderNodeTypes::SimpleShaderNode
//					, glm::vec3(4, 0, 0));
//
	createObject(CubeDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(4, 0, 0));

//	createObject(SphereColObj
//					, ShaderNodeTypes::SimpleShaderNode
//					, glm::vec3(0, 0, 0));

	createObject(SphereDirLightObj
					, ShaderNodeTypes::DirLightShaderNode
					, glm::vec3(-4, 0, 0));

	createObject(SpherePntLightObj
					, ShaderNodeTypes::PntLightShaderNode
					, glm::vec3(0, 0, 0));

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
		case CubeColObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::CubeSimpleVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
																		, mObjectMeshes.at(MeshType::Cube)->elementNum));
			break;
		}
		case CubeDirLightObj:
		case CubePntLightObj:
		{
			OGLRenderer::VertexArrays vao;

			if(objType == OGLRenderer::Programs::DirLightProgram)
				vao = OGLRenderer::VertexArrays::CubeDirLightVAO;
			else
				vao = OGLRenderer::VertexArrays::CubePntLightVAO;

			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, vao
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
																		, mObjectMeshes.at(MeshType::Cube)->elementNum
																		, mRenderer.getTex(OGLRenderer::Textures::WoodBoxTex)
																		, mRenderer.getTexTarget(OGLRenderer::Textures::WoodBoxTex)));

			break;
		}
		case SphereColObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthSimpleVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
																		, mObjectMeshes.at(MeshType::Earth)->elementNum));

			break;
		}
		case SphereDirLightObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthDirLightVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
																		, mObjectMeshes.at(MeshType::Earth)->elementNum
																		, mRenderer.getTex(OGLRenderer::Textures::BlueMarbleTex)
																		, mRenderer.getTexTarget(OGLRenderer::Textures::BlueMarbleTex)));

			break;
		}
		case SpherePntLightObj:
		{
			obj = std::unique_ptr<SceneNode>(new SceneNode(Node::NodeType::ScNode
																		, mRenderer
																		, mSceneGraphShaderNodes.at(objShdr)
																		, objPos
																		, objScale
																		, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthPntLightVAO)
																		, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
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
															, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
															, mObjectMeshes.at(MeshType::Earth)->elementNum));

			break;
		}
		default: break;
	}

	// Set position of object
//	obj->setPosition(objPos);

	// Store in scene graph
	mSceneGraphShaderNodes.at(objShdr)->attachChild(std::move(obj));
}

void World::drawWorld()
{
   mRenderer.clearContext();

	mSceneGraph.draw(mRenderer.getVMatrix()
							, mRenderer.getPMatrix());
}
