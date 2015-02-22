#ifndef SHADERNODE_HPP
#define SHADERNODE_HPP

#include "Node.hpp"
#include "OGLRenderer.hpp"

//class Shader;
//class OGLRenderer;

class ShaderNode : public Node
{
private:
	OGLRenderer					&mRenderer;
	Shader						*mShaderInfo;
	OGLRenderer::Programs	mProgramType;

	int 							mShaderID;

public:
									ShaderNode(Node::NodeType nodeType
												  , OGLRenderer &renderer
												  , Shader *shdrInf
												  , OGLRenderer::Programs progType
												  , int shaderID)
									: Node(nodeType)
									, mRenderer(renderer)
									, mShaderInfo(shdrInf)
									, mProgramType(progType)
									, mShaderID(shaderID)
									{

									};

	virtual 						~ShaderNode(){};

	virtual void 				draw(glm::mat4 V
										 , glm::mat4 P);

	Shader*						getShaderInfo()
									{ return mShaderInfo; }

	OGLRenderer::Programs	getProgramType()
									{ return mProgramType; }

	int 							getShaderID()
									{ return mShaderID; }
};

#endif // SHADERNODE_HPP
