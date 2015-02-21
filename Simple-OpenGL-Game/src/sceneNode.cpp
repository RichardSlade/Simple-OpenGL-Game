#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <SFML/OpenGL.hpp>

#include <GL/glew.h>

#include "SceneNode.hpp"

SceneNode::SceneNode(Node::NodeType nodeType
							, OGLRenderer& renderer
							, ShaderNode* shdrNode
							, GLuint VAO
							, GLuint EBO
							, GLuint numEle
//							, Shader *shdrInf
							, GLint mvpID
							, GLint samplerID
							, GLuint texID
							, GLenum texTarget)
: Node(nodeType)
, mOGLRenderer(renderer)
, mShaderNode(shdrNode)
, mVAO(VAO)
, mEBO(EBO)
, mTexID(texID)
, mTexTarget(texTarget)
, mNumElements(numEle)
, mMVPID(mvpID)
, mSamplerID(samplerID)
, mPosition(glm::vec3(0.f))
, mRotation(glm::vec3(0.f))
, mScale(glm::vec3(0.f))
{
	// Translate object
	mTranslationMatrix = glm::translate(mPosition);

	// Rotate object
	mRotationMatrix = glm::rotate(mRotation.x, glm::vec3(1.f, 0.f, 0.f));
	mRotationMatrix *= glm::rotate(mRotation.y, glm::vec3(0.f, 1.f, 0.f));
	mRotationMatrix *= glm::rotate(mRotation.z, glm::vec3(0.f, 0.f, 1.f));

	// Scale object
	mScalingMatrix = glm::scale(mScale);

	// Combine transformations
//	mModelMatrix = mRotationMatrix
//					* mScalingMatrix
//					* mTranslationMatrix;

	mModelMatrix = mTranslationMatrix;
}

//SceneNode::SceneNode(Node::NodeType nodeType
//							, OGLRenderer& renderer
//							, ShaderNode* shdrNode
//							, GLuint VAO
//							, GLuint EBO
//							, GLuint numEle
//							, GLuint texID
//							, GLenum texTarget)
//: Node(nodeType)
//, mOGLRenderer(renderer)
//, mShaderNode(shdrNode)
//, mVAO(VAO)
//, mEBO(EBO)
//, mNumElements(numEle)
//, mTexID(texID)
//, mTexTarget(texTarget)
//, mPosition(glm::vec3(0.f))
//, mRotation(glm::vec3(0.f))
//, mScale(glm::vec3(0.f))
//{
//	// Translate object
//	mTranslationMatrix = glm::translate(mPosition);
//
//	// Rotate object
//	mRotationMatrix = glm::rotate(mRotation.x, glm::vec3(1.f, 0.f, 0.f));
//	mRotationMatrix *= glm::rotate(mRotation.y, glm::vec3(0.f, 1.f, 0.f));
//	mRotationMatrix *= glm::rotate(mRotation.z, glm::vec3(0.f, 0.f, 1.f));
//
//	// Scale object
//	mScalingMatrix = glm::scale(mScale);
//
//	// Combine transformations
////	mModelMatrix = mRotationMatrix
////					* mScalingMatrix
////					* mTranslationMatrix;
//
//	mModelMatrix = mTranslationMatrix;
//}

void SceneNode::drawCurrent(glm::mat4 VP)
{
//	std::cout << "Draw SCENE NODE" << std::endl;

	glm::mat4 MVP = VP * mModelMatrix;
//	glm::mat4 MVP = VP * glm::mat4(1);

	mOGLRenderer.draw(mVAO
						 , mEBO
						 , mNumElements
						 , mTexID
						 , mTexTarget
						 , mSamplerID
						 , mMVPID
						 , MVP);
}

void SceneNode::draw(glm::mat4 VP)
{
	drawCurrent(VP);

	for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->draw(VP);
}

void SceneNode::setPosition(glm::vec3 pos)
{
	mPosition += pos;

	mTranslationMatrix = glm::translate(pos);

//	mModelMatrix = mRotationMatrix
//						* mScalingMatrix
//						* mTranslationMatrix;

	mModelMatrix = mTranslationMatrix;
}


