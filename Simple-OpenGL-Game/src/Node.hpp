#ifndef NODE_HPP
#define NODE_HPP

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Node
{
public:
	typedef std::unique_ptr<Node> NodeUPtr;

	enum NodeType
	{
		ShdrNode,
		Root,
		ScNode,
		NumNodeTypes
	};

protected:
	NodeType 							mType;

	Node*									mParent;
	std::vector<NodeUPtr> 			mChildren;

public:
										Node(NodeType nodeType)
										: mType(nodeType)
										{

										};

	virtual 							~Node(){};

	virtual void 					draw();

	virtual void 					depthPass();

	void 								attachChild(NodeUPtr node);
	void 								removeChild();

	// Getters
	NodeType 						getType()
										{ return mType; }

	Node*								getParent()
										{ return mParent; }

	// Setters
	void								setParent(Node* parent)
										{ mParent = parent; }
};

#endif // NODE_HPP
