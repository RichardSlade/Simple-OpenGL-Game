#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

//#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct LightSource
{
//	bool 				isEnabled;
//	int 				type;

//	glm::vec3		ambient;
//	glm::vec3		colour;
	glm::vec3 		position;
	glm::vec3      viewDirection;
//	glm::vec3 		halfVector;

	glm::mat4		V;
	glm::mat4		P;

						LightSource(){};

						LightSource(glm::vec3 pos
										, glm::vec3 viewDir)
						: position(pos)
						, viewDirection(viewDir)
						{
							V = glm::lookAt(pos, viewDir, glm::vec3(0.f, 1.f, 0.f));
							P = glm::ortho<float>(-50.f, 50.f, -50.f, 50.f, 1.f, 100.f);
//							P = glm::frustum<float>(-50.f, 50.f, -50.f, 50.f, 1.f, 50.f);
						};
};

#endif // LIGHTSOURCE_HPP
