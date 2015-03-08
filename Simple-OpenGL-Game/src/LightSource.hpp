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
							V = glm::lookAt(pos, viewDir, glm::vec3(0, 1, 0));
							P = glm::ortho<float>(-10.f, 10.f, -10.f, 10.f, -10.f, 20.f);
						};
};

#endif // LIGHTSOURCE_HPP
