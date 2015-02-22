#ifndef POINTSOURCE_HPP
#define POINTSOURCE_HPP

#include "LightSource.hpp"

struct PointSource : public LightSource
{
	float			constantAttenuation;
	float			linearAttenuation;
	float 			quadraticAttenuation;

					PointSource(){};

					PointSource(bool isOn
								, int t
								, glm::vec3 A
								, glm::vec3 C
								, glm::vec3 pos
								, float cnstAtt
								, float linAtt
								, float quadAtt)
					: LightSource(isOn, t, A, C, pos)
					, constantAttenuation(cnstAtt)
					, linearAttenuation(linAtt)
					, quadraticAttenuation(quadAtt)
					{};
};

#endif // POINTSOURCE_HPP
