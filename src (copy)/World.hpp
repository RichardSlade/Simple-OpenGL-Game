#ifndef WORLD_HPP
#define WORLD_HPP

#include "OBJMesh.hpp"
//#include "SceneNode.hpp"
#include "ShaderNode.hpp"

class OGLRenderer;

class World
{
public:
	typedef std::unique_ptr<World> WorldUPtr;

	enum MeshType
	{
		Cube,
		Earth,
		NumMeshes
	};

	enum ShaderNodes
	{
		SimpleShaderNode,
		Tex2DShaderNode,
		DirLightShaderNode,
		NumShaderNodes
	};

private:
	OGLRenderer							&mRenderer;

	std::vector<OBJMesh::OBJMeshPtr>	mObjectMeshes;

	Node									mSceneGraph;
	std::vector<ShaderNode*>		mSceneGraphShaderNodes;

	void								loadObjects();
	void 								buildWorld();

public:
										World(OGLRenderer &renderer);

	void 								display();

	// Getters
	OBJMesh*							getObject(MeshType type)
										{ return mObjectMeshes.at(type).get(); }
};

#endif // WORLD_HPP
