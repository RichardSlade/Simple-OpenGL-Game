#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <SFML/OpenGL.hpp>

#include <GL/glew.h>

#include "SceneNode.hpp"
#include "ShaderNode.hpp"

SceneNode::SceneNode(Node::NodeType nodeType
							, OGLRenderer& renderer
							, ShaderNode* shdrNode
							, glm::vec3 pos
							, glm::vec3 scale
							, GLuint VAO
							, GLuint EBO
							, GLuint numEle
							, GLuint texID
							, GLenum texTarget)
: Node(nodeType)
, mOGLRenderer(renderer)
, mShaderNode(shdrNode)
, mVAO(VAO)
, mEBO(EBO)
, mNumElements(numEle)
, mTexID(texID)
, mTexTarget(texTarget)
, mPosition(pos)
, mRotation(glm::vec3(0.f))
{
	// Translate object
	mTranslationMatrix = glm::translate(mPosition);

	// Rotate object
//	mRotationMatrix = glm::rotate(mRotation.x, glm::vec3(1.f, 0.f, 0.f));
//	mRotationMatrix *= glm::rotate(mRotation.y, glm::vec3(0.f, 1.f, 0.f));
//	mRotationMatrix *= glm::rotate(mRotation.z, glm::vec3(0.f, 0.f, 1.f));

	mRotationMatrix = glm::mat4(1);

	// Scale object
	mScalingMatrix = glm::scale(scale);

	// Combine transformations
	mModelMatrix = mRotationMatrix * mScalingMatrix * mTranslationMatrix;
}

void SceneNode::drawCurrent(glm::mat4 V
									, glm::mat4 P)
{
//	std::cout << "Draw SCENE NODE" << std::endl;

	glm::mat4 MV = V * mModelMatrix;
	glm::mat4 MVP = P * V * mModelMatrix;

//	glm::mat4 MVP = VP * glm::mat4(1);

	mOGLRenderer.draw(MVP
						 , MV
						 , mVAO
						 , mEBO
						 , mNumElements
						 , mTexID
						 , mTexTarget
						 , mShaderNode->getShaderInfo()
						 , mShaderNode->getProgramType()
//						 , mSamplerID
//						 , mMVPID
						  );
}

void SceneNode::draw(glm::mat4 V
							, glm::mat4 P)
{
	drawCurrent(V, P);

	for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->draw(V, P);
}

void SceneNode::setPosition(glm::vec3 pos)
{
	mPosition += pos;

//	mTranslationMatrix = glm::translate(pos);

//	mModelMatrix = mRotationMatrix
//						* mScalingMatrix
//						* mTranslationMatrix;

//	mModelMatrix = mTranslationMatrix;
}


