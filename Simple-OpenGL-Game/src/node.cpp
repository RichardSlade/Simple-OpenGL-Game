#include <iostream>

#include "Node.hpp"

void Node::draw(glm::mat4 VP)
{
//	std::cout << "Draw ROOT" << std::endl;

	for(unsigned int i = 0; i < mChildren.size(); i++)
		mChildren.at(i)->draw(VP);
}

void Node::attachChild(NodeUPtr node)
{
	node->setParent(this);
//	node->setShaderID(mNodeShaderID);
	mChildren.push_back(std::move(node));
}

void Node::removeChild()
{

}
