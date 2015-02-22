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

	enum ShaderNodeTypes
	{
		SimpleShaderNode,
		Tex2DShaderNode,
		DirLightShaderNode,
		PntLightShaderNode,
		NumShaderNodeTypes
	};

	enum ObjectTypes
	{
		CubeColObj,
		CubeDirLightObj,
		CubePntLightObj,
		SphereColObj,
		SphereDirLightObj,
		SpherePntLightObj,
		LightObj,
		NumObjectTypes
	};

private:
	OGLRenderer							&mRenderer;

	std::vector<OBJMesh::OBJMeshPtr>	mObjectMeshes;

	Node									mSceneGraph;
	std::vector<ShaderNode*>		mSceneGraphShaderNodes;

	glm::vec3						mDirLightSource;
	std::vector<glm::vec3>		mPntLightSources;

	void								loadObjects();
	void 								buildWorld();

	void 								createObject(ObjectTypes objType
														, ShaderNodeTypes objShdr
														, glm::vec3 objPos
														, glm::vec3 objScale = glm::vec3(1));

public:
										World(OGLRenderer &renderer);

	void 								drawWorld();

	// Getters
	OBJMesh*							getObject(MeshType type)
										{ return mObjectMeshes.at(type).get(); }
};

#endif // WORLD_HPP
