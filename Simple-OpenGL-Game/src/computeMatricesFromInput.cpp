#include "ComputeMatricesFromInput.hpp"

#include <cmath>
#include <iostream>

static glm::vec3 position = glm::vec3(0, 10, 15);
static float horizontalAngle = -2.5f;
//static float horizontalAngle = 0.f;
static float verticalAngle = -0.4f;
static float initialFoV = 45.f;
static float speed = 4.f;
static float mouseSpeed = 0.04f;

glm::mat4 CubeMapModelMatrix;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::vec3 ViewDirection;

glm::mat4 getCubeMapModelMatrix()
{
	return CubeMapModelMatrix;
}

glm::mat4 getViewMatrix()
{
	return ViewMatrix;
}

glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

glm::vec3 getViewDirection()
{
	return ViewDirection;
}

void computeMatricesFromInput(sf::RenderWindow *window, sf::Time dt)
{
	// Get mouse position
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

	sf::Vector2u windowSize = window->getSize();
	sf::Vector2i newMousePos(windowSize.x / 2.f, windowSize.y / 2.f);

	sf::Mouse::setPosition(newMousePos, *window);

	float deltaTime = dt.asSeconds();

	float mouseMoveX = (newMousePos.x - mousePos.x);
	float mouseMoveY = (newMousePos.y - mousePos.y);

	// Compute new orientation
	horizontalAngle += mouseSpeed * deltaTime * mouseMoveX;
	verticalAngle   += mouseSpeed * deltaTime * mouseMoveY;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	ViewDirection = glm::vec3(
		std::cos(verticalAngle) * std::sin(horizontalAngle),
		std::sin(verticalAngle),
		std::cos(verticalAngle) * std::cos(horizontalAngle)
	);

//	glm::vec3 direction = glm::vec3(
//		std::cos(0.f) * std::sin(horizontalAngle),
//		std::sin(0.f),
//		std::cos(0.f) * std::cos(horizontalAngle)
//	);

	glm::vec3 direction = ViewDirection;

	// Right vector
	glm::vec3 right = glm::vec3(
		std::sin(horizontalAngle - 3.14f/2.0f),
		0,
		std::cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		position += direction * deltaTime * speed;
	}
	// Move backward
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		position -= direction * deltaTime * speed;
	}

	// Strafe right
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		position += right * deltaTime * speed;
	}
	// Strafe left
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	CubeMapModelMatrix = glm::translate(position);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position + direction, // and looks here : at the same position, plus "direction"
								up);                 // Head is up (set to 0,-1,0 to look upside-down)

//	CubeMapModelMatrix = glm::translate(direction);
}


