#include <iostream>

#include "ShaderNode.hpp"
#include "OGLRenderer.hpp"

void ShaderNode::draw()
{
	mRenderer.setupShader(mShaderID);

   for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->draw();
}

void ShaderNode::depthPass()
{
	// No need to set shader as world class has
	// already used setupShadowShader()
	for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->depthPass();
}
