#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

//#include <fstream>


#include <glm/glm.hpp>

struct LightSource
{
	bool 				isEnabled;
	int 				type;

	glm::vec3		ambient;
	glm::vec3		colour;
	glm::vec3 		position;
	glm::vec3 		halfVector;

						LightSource(){};

						LightSource(bool isE
										, int t
										, glm::vec3 A
										, glm::vec3 C
										, glm::vec3 pos)

						: isEnabled(isE)
						, type(t)
						, ambient(A)
						, colour(C)
						, position(pos)
						, halfVector(glm::vec3(0))
						{

						};
};

#endif // LIGHTSOURCE_HPP
