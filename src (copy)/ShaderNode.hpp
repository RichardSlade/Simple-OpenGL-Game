#ifndef SHADERNODE_HPP
#define SHADERNODE_HPP

#include "Node.hpp"

class Shader;
class OGLRenderer;

class ShaderNode : public Node
{
private:
	OGLRenderer			&mRenderer;
	Shader				*mShaderInfo;

	int 					mShaderID;

public:
							ShaderNode(OGLRenderer &renderer
										  , Node::NodeType nodeType
										  , int shaderID)
							: Node(nodeType)
							, mRenderer(renderer)
							, mShaderID(shaderID)
							{

							};

	virtual 				~ShaderNode(){};

	virtual void 		draw(glm::mat4 VP);

	int 					getShaderID()
							{ return mShaderID; }

	Shader*				getShaderInfo()
							{ return mShaderInfo; }
};

#endif // SHADERNODE_HPP
