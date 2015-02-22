#include <iostream>

#include "ShaderNode.hpp"
#include "OGLRenderer.hpp"

void ShaderNode::draw(glm::mat4 V
							, glm::mat4 P)
{
//	std::cout << "Draw SHADER NODE: " << mShaderID << std::endl;

	mRenderer.setupShader(mShaderID);

   for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->draw(V, P);
}
