#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Node.hpp"
#include "OGLRenderer.hpp"

class ShaderNode;

class SceneNode : public Node
{
private:
	OGLRenderer							*mOGLRenderer;
	ShaderNode							*mShaderNode;

	GLuint 								mVAO;
	GLuint								mDepthVAO;
	GLuint 								mEBO;
	GLuint 								mNumElements;

	GLuint								mTexID;
	GLenum								mTexTarget;

//	GLint 								mMVPID;
//	GLint 								mSamplerID;

	glm::vec3 							mPosition;
	glm::vec3							mRotation;
	glm::vec3							mScale;

	glm::mat4							mTranslationMatrix;
	glm::mat4							mRotationMatrix;
	glm::mat4							mScalingMatrix;

	glm::mat4							mModelMatrix;

	void 									drawCurrent();

public:
											SceneNode(NodeType nodeType
														, glm::vec3 pos)
											: Node(nodeType)
											, mPosition(pos)
											, mOGLRenderer(nullptr)
											, mShaderNode(nullptr)
											, mVAO(0)
											, mDepthVAO(0)
											, mEBO(0)
											, mNumElements(0)
											, mTexID(0)
											, mTexTarget(0)
											{};

											SceneNode(NodeType nodeType
														, OGLRenderer *renderer
														, ShaderNode *shdrNode
														, glm::vec3 pos
														, glm::vec3 scale
														, GLuint VAO
														, GLuint depthVAO
														, GLuint EBO
														, GLuint numEle
														, GLuint texID = 0
														, GLenum texTarget = GL_TEXTURE0);

	virtual 							~SceneNode(){};

	virtual void 					draw();

	virtual void 					depthPass();

	// Getters
	const glm::vec3 				getPosition()
										{return mPosition;}

	// Setters
	void 								setPosition(glm::vec3 pos);
};

#endif // SCENENODE_HPP
