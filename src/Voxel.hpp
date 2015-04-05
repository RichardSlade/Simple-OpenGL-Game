#ifndef VOXEL_HPP
#define VOXEL_HPP

#include <glm/glm.hpp>

struct Voxel
{
public:
	float 				Top;
	float 				Bottom;
	float 				Left;
	float 				Right;
	float 				Front;
	float				Back;

						Voxel(float top
							 , float bottom
							 , float left
							 , float right
							 , float front
							 , float back)
						: Top(top)
						, Bottom(bottom)
						, Left(left)
						, Right(right)
						, Front(front)
						, Back(back)
						{};

	bool				contains(glm::vec3 pos)
						{
							if((pos.x < Right && pos.x > Left)
								&& (pos.y < Top && pos.y > Bottom)
								&& (pos.z < Front && pos.z > Back))
								return true;
							else
								return false;
						}
};

#endif // VOXEL_HPP
