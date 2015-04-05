#ifndef OCTTREE_HPP
#define OCTTREE_HPP

//#include <array>
#include <vector>


#include <glm/glm.hpp>
#include "OctTreePartition.hpp"

class OctTree
{
public:
	typedef std::vector<OctTreePartition> FirstDim;
	typedef std::vector<FirstDim> SecondDim;
	typedef std::vector<SecondDim> Partitions;

private:
	glm::vec3		mWorldDim;
//	int 				mDivider;
	float 				mXDiv;
	float 				mYDiv;
	float 				mZDiv;


	Partitions			mPartitions;

public:
						OctTree(glm::vec3 worldDim
								, float div);

	OctTreePartition 	getLocalEntities(glm::vec3 pos);

	void 				insertInto(SceneNode* entity);

	// Getters
	glm::vec3 		getWorldDim()
							{ return mWorldDim; }
};

#endif // OCTTREE_HPP
