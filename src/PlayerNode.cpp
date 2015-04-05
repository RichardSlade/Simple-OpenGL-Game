#include "PlayerNode.hpp"

void PlayerNode::handleInput(sf::Time dt)
{
   mInputHandler->computePlayerMatrices(dt);
}
