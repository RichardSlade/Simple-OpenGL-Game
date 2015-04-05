#include "PlayerInput.hpp"
#include "PlayerNode.hpp"

void PlayerInput::computePlayerMatrices(sf::Time dt)
{
	// Get mouse position
	sf::Vector2i mousePos = sf::Mouse::getPosition(*mWindow);

	sf::Vector2u windowSize = mWindow->getSize();
	sf::Vector2i newMousePos(windowSize.x / 2.f, windowSize.y / 2.f);

	sf::Mouse::setPosition(newMousePos, *mWindow);

	float deltaTime = sf::seconds(1.f / 60.f).asSeconds();
//	float deltaTime = dt.asSeconds();

	float mouseMoveX = (newMousePos.x - mousePos.x);
	float mouseMoveY = (newMousePos.y - mousePos.y);

	// Compute new orientation
	mHorizontalAngle += mMouseSpeed * deltaTime * mouseMoveX;
	mVerticalAngle   += mMouseSpeed * deltaTime * mouseMoveY;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	mViewDirection = glm::vec3(
		std::cos(mVerticalAngle) * std::sin(mHorizontalAngle),
		std::sin(mVerticalAngle),
		std::cos(mVerticalAngle) * std::cos(mHorizontalAngle)
	);

	glm::vec3 direction = glm::vec3(
		std::cos(0.f) * std::sin(mHorizontalAngle),
		std::sin(0.f),
		std::cos(0.f) * std::cos(mHorizontalAngle)
	);

	direction = mViewDirection;

	// Right vector
	glm::vec3 right = glm::vec3(
		std::sin(mHorizontalAngle - 3.14f / 2.0f),
		0,
		std::cos(mHorizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
//		position += direction * deltaTime * mSpeed;
		mPlayer.setPosition(direction * deltaTime * mSpeed);
	}
	// Move backward
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
//		position -= direction * deltaTime * mSpeed;
		mPlayer.setPosition(-(direction * deltaTime * mSpeed));
	}

	// Strafe right
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
//		position += right * deltaTime * mSpeed;
		mPlayer.setPosition(right * deltaTime * mSpeed);
	}
	// Strafe left
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
//		position -= right * deltaTime * mSpeed;
		mPlayer.setPosition(-(right * deltaTime * mSpeed));
	}

	float FoV = mInitialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

//	CubeMapModelMatrix = glm::translate(mPlayer.getPosition());

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mProjMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	mViewMatrix       = glm::lookAt(
								mPlayer.getPosition(),           // Camera is here
								mPlayer.getPosition() + mViewDirection, // and looks here : at the same position, plus "direction"
								up);                 // Head is up (set to 0,-1,0 to look upside-down)
}
