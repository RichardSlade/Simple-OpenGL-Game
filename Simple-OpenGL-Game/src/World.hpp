#ifndef WORLD_HPP
#define WORLD_HPP

#include "OBJMesh.hpp"
//#include "SceneNode.hpp"
#include "ShaderNode.hpp"
#include "LightSource.hpp"

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
//		Tex2DShaderNode,
		DirLightShaderNode,
//		PntLightShaderNode,
//		ShadowShaderNode,
		NumShaderNodeTypes
	};

	enum ObjectTypes
	{
//		CubeColObj,
		CubeDirLightObj,
//		CubePntLightObj,
//		SphereColObj,
		SphereDirLightObj,
//		SpherePntLightObj,
		LightObj,
		FloorDirLightObj,
		NumObjectTypes
	};

private:
	OGLRenderer							&mRenderer;

	std::vector<OBJMesh::OBJMeshPtr>	mObjectMeshes;

	Node									mSceneGraph;
	std::vector<ShaderNode*>		mSceneGraphShaderNodes;

	LightSource						mDirLightSource;
	std::vector<glm::vec3>		mPntLightSources;

	glm::mat4 						mLightViewMatrix;
	glm::mat4 						mLightProjMatrix;

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
