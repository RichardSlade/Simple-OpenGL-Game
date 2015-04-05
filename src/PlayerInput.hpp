#ifndef PLAYERINPUT_HPP
#define PLAYERINPUT_HPP

//#include "PlayerNode.hpp"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class PlayerNode;

class PlayerInput
{
private:
	PlayerNode 					&mPlayer;
	sf::RenderWindow			*mWindow;

	glm::mat4 					mViewMatrix;
	glm::mat4 					mProjMatrix;

	glm::vec3 					mViewDirection;

	float 						mHorizontalAngle;
	float 						mVerticalAngle;
	float 						mInitialFoV;
	float 						mSpeed;
	float 						mMouseSpeed;

public:
									PlayerInput(PlayerNode &player
													, sf::RenderWindow *window)
									: mPlayer(player)
									, mWindow(window)
									, mHorizontalAngle(3.f)
									, mVerticalAngle(0.f)
									, mInitialFoV(45.f)
									, mSpeed(8.f)
									, mMouseSpeed(0.04f)
									{};


	void 							computePlayerMatrices(sf::Time dt);

	// Getters
	const glm::mat4 			getViewMatrix()
									{ return mViewMatrix; }

	const glm::mat4 			getProjMatrix()
									{ return mProjMatrix; }

	const glm::vec3 			getViewDirection()
									{ return mViewDirection; }
};

#endif // PLAYERINPUT_HPP
