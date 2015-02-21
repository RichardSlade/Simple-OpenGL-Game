#ifndef COMPUTEMATRICESFROMINPUT_HPP
#define COMPUTEMATRICESFROMINPUT_HPP

#include <memory>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

glm::mat4 getCubeMapModelMatrix();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

glm::vec3 getViewDirection();

void computeMatricesFromInput(sf::RenderWindow *window, sf::Time dt);

#endif // COMPUTEMATRICESFROMINPUT_HPP
