#ifndef OCTTREEPARTITION_HPP
#define OCTTREEPARTITION_HPP

#include <list>

#include "Voxel.hpp"

class SceneNode;

class OctTreePartition
{
 private:
   Voxel                        mVoxel;

 	std::list<SceneNode*>       mEntities;
// 	std::vector<OctTreePartition> mChildren;

public:
                                OctTreePartition(Voxel voxel)
                                : mVoxel(voxel)
                                {};

	void 						insertEntity(SceneNode* entity);
	void 					    deleteEntity(SceneNode* entity);

	// Getters
	std::list<SceneNode*>       getEntities()
                                { return mEntities; }

};

#endif // OCTTREEPARTITION_HPP
