#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Node.hpp"
#include "OGLRenderer.hpp"

class ShaderNode;

class SceneNode : public Node
{
private:
	OGLRenderer							&mOGLRenderer;
	ShaderNode							*mShaderNode;
//	Shader								*mShaderInfo;

	GLuint 								mVAO;
	GLuint 								mEBO;
	GLuint 								mNumElements;

	GLuint								mTexID;
	GLenum								mTexTarget;

	GLint 								mMVPID;
	GLint 								mSamplerID;

	glm::vec3 							mPosition;
	glm::vec3							mRotation;
//	glm::vec3							mScale;

	glm::mat4							mTranslationMatrix;
	glm::mat4							mRotationMatrix;
	glm::mat4							mScalingMatrix;

	glm::mat4							mModelMatrix;

	void 									drawCurrent(glm::mat4 V
														  , glm::mat4 P);

public:
											SceneNode(NodeType nodeType
														, OGLRenderer &renderer
														, ShaderNode *shdrNode
														, glm::vec3 pos
														, glm::vec3 scale
														, GLuint VAO
														, GLuint EBO
														, GLuint numEle
														, GLuint texID = 0
														, GLenum texTarget = GL_TEXTURE0);

//											SceneNode(NodeType nodeType
//														, OGLRenderer &renderer
//														, ShaderNode *shdrNode
//														, GLuint VAO
//														, GLuint EBO
//														, GLuint numEle
//														, GLint mvpID
//														, GLint samplerID = 0
//														, GLuint texID = 0
//														, GLenum texTarget = GL_TEXTURE0);

	virtual 							~SceneNode(){};

	virtual void 					draw(glm::mat4 V
												, glm::mat4 P);

	// Setters
	void 								setPosition(glm::vec3 pos);
};

#endif // SCENENODE_HPP
