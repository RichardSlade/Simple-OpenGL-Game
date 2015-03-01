#include <iostream>

#include "World.hpp"
#include "OGLRenderer.hpp"
#include "Texture2DShader.hpp"
#include "DirectionalLightingShaderTex.hpp"
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
}

void World::loadObjects()
{
	try
	{
		mObjectMeshes.push_back(OBJMesh::OBJMeshPtr(new OBJMesh("Cube"
																					,"models/cubeWoodBox.obj"
																					, Mesh::ShadingType::Gouraud)));

		mObjectMeshes.push_back(OBJMesh::OBJMeshPtr(new OBJMesh("Earth"
																, "models/earth.obj"
																, Mesh::ShadingType::Gouraud)));
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
		std::unique_ptr<ShaderNode> shaderNode(new ShaderNode(mRenderer
															 , static_cast<Node::NodeType>(i)
															 , mRenderer.getShaderProgramID(i)));

		mSceneGraphShaderNodes.push_back(dynamic_cast<ShaderNode*>(shaderNode.get()));
		mSceneGraph.attachChild(std::move(shaderNode));
	}

	Texture2DShader *tex2DShdr = dynamic_cast<Texture2DShader*>(mRenderer.getShaderInfo(OGLRenderer::Programs::Texture2DProgram));
	DirectionalLightingShaderTex *dirLightShdr = dynamic_cast<DirectionalLightingShaderTex*>(mRenderer.getShaderInfo(OGLRenderer::Programs::DirectionLightingProgram));

	// Create cube objects
	std::unique_ptr<SceneNode> cube(new SceneNode(Node::NodeType::ScNode
												, mRenderer
												, mSceneGraphShaderNodes.at(ShaderNodes::SimpleShaderNode)
												, mRenderer.getVAO(OGLRenderer::VertexArrays::CubeSimpleVAO)
												, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
												, mObjectMeshes.at(MeshType::Cube)->elementNum
												, mRenderer.getShaderInfo(OGLRenderer::Programs::SimpleProgram)->MVP));

	mSceneGraphShaderNodes.at(ShaderNodes::SimpleShaderNode)->attachChild(std::move(cube));


	std::unique_ptr<SceneNode> cubeTex(new SceneNode(Node::NodeType::ScNode
													, mRenderer
													, mSceneGraphShaderNodes.at(ShaderNodes::Tex2DShaderNode)
													, mRenderer.getVAO(OGLRenderer::VertexArrays::CubeTexVAO)
													, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::CubeEBO)
													, mObjectMeshes.at(MeshType::Cube)->elementNum
													, tex2DShdr->MVP
													, tex2DShdr->Sampler
													, mRenderer.getTex(OGLRenderer::Textures::WoodBoxTex)
//													, GL_TEXTURE0));
													, mRenderer.getTexTarget(OGLRenderer::Textures::WoodBoxTex)));

	cubeTex->setPosition(glm::vec3(-4.f, 0.f, 0.f));

	mSceneGraphShaderNodes.at(ShaderNodes::Tex2DShaderNode)->attachChild(std::move(cubeTex));

	// Create earth objects
	std::unique_ptr<SceneNode> earth(new SceneNode(Node::NodeType::ScNode
												, mRenderer
												, mSceneGraphShaderNodes.at(ShaderNodes::SimpleShaderNode)
												, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthSimpleVAO)
												, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
												, mObjectMeshes.at(MeshType::Earth)->elementNum
												, mRenderer.getShaderInfo(OGLRenderer::Programs::SimpleProgram)->MVP));

	earth->setPosition(glm::vec3(4.f, 0.f, 0.f));

	mSceneGraphShaderNodes.at(ShaderNodes::SimpleShaderNode)->attachChild(std::move(earth));

	std::unique_ptr<SceneNode> earthTex(new SceneNode(Node::NodeType::ScNode
													, mRenderer
													, mSceneGraphShaderNodes.at(ShaderNodes::Tex2DShaderNode)
													, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthTexVAO)
													, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
													, mObjectMeshes.at(MeshType::Earth)->elementNum
													, tex2DShdr->MVP
													, tex2DShdr->Sampler
													, mRenderer.getTex(OGLRenderer::Textures::BlueMarbleTex)
													, mRenderer.getTexTarget(OGLRenderer::Textures::BlueMarbleTex)));

	earthTex->setPosition(glm::vec3(8.f, 0.f, 0.f));

	mSceneGraphShaderNodes.at(ShaderNodes::Tex2DShaderNode)->attachChild(std::move(earthTex));

//	std::unique_ptr<SceneNode> earthTexDirLighting(new SceneNode(Node::NodeType::ScNode
//													, mRenderer
//													, mSceneGraphShaderNodes.at(ShaderNodes::DirLightShaderNode)
//													, mRenderer.getVAO(OGLRenderer::VertexArrays::EarthDirLightVAO)
//													, mRenderer.getEBO(OGLRenderer::ElementArrayBuffers::EarthEBO)
//													, mObjectMeshes.at(MeshType::Earth)->elementNum
//													, dirLightShdr->MVP
//													, dirLightShdr->Sampler
//													, mRenderer.getTex(OGLRenderer::Textures::BlueMarbleTex)
//													, mRenderer.getTexTarget(OGLRenderer::Textures::BlueMarbleTex)));
//
//	earthTexDirLighting->setPosition(glm::vec3(12.f, 0.f, 0.f));
//
//	mSceneGraphShaderNodes.at(ShaderNodes::DirLightShaderNode)->attachChild(std::move(earthTexDirLighting));
}

void World::display()
{
   mRenderer.clearContext();

	mSceneGraph.draw(mRenderer.getVPMatrix());
}
