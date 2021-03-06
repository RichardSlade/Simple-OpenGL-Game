#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Node.hpp"

void Node::draw()
{
	for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->draw();
}

void Node::depthPass()
{
	for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->depthPass();
}

void Node::attachChild(NodeUPtr node)
{
	node->setParent(this);
	mChildren.push_back(std::move(node));
}

void Node::removeChild()
{

}
