#ifndef PLAYERNODE_HPP
#define PLAYERNODE_HPP

#include <SFML/System/Time.hpp>

//#include "Node.hpp"
#include "SceneNode.hpp"
#include "PlayerInput.hpp"

class PlayerNode : public SceneNode
{
private:
	std::unique_ptr<PlayerInput> 	mInputHandler;

	public:
											PlayerNode(sf::RenderWindow *window
														, NodeType nodeType
														, glm::vec3 pos)
											: SceneNode(nodeType, pos)
											, mInputHandler(new PlayerInput(*this, window))
											{};

	virtual 								~PlayerNode(){};

	virtual void						draw(){};
	virtual void 						depthPass(){};

	void 									handleInput(sf::Time dt);

	// Getters
	PlayerInput*						getPlayerInput()
											{ return mInputHandler.get(); }
};

#endif // PLAYERNODE_HPP
