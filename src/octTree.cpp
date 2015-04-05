#include <iostream>

#include "OctTree.hpp"
#include "SceneNode.hpp"

OctTree::OctTree(glm::vec3 worldDim
				, float div)
: mWorldDim(worldDim)
//, mDivider(div)
, mXDiv(worldDim.x / div)
, mYDiv(worldDim.y / div)
, mZDiv(worldDim.z / div)
{
	glm::vec3 pos(0, 0 ,0);

	for(unsigned int y = 0; y < div; y++)
	{
		mPartitions.push_back(SecondDim());

		for(unsigned int x = 0; x < div; x++)
		{
			mPartitions.at(y).push_back(FirstDim());

			for(unsigned int z = 0; z < div; z++)
			{
				Voxel voxel(pos.x, pos.x + mXDiv
							, pos.y, pos.y + mYDiv
							, pos.z, pos.z + mZDiv);

				mPartitions.at(y).at(x).push_back(OctTreePartition(voxel));

				pos.z += mZDiv;
			}

			pos.x += mXDiv;
		}

		pos.y += mYDiv;
	}
}

OctTreePartition OctTree::getLocalEntities(glm::vec3 pos)
{
	glm::vec3 index;

	index.x = pos.x / mXDiv;
	index.y = pos.y / mYDiv;
	index.z = pos.z / mZDiv;

	return mPartitions.at(index.x).at(index.y).at(index.z);
}

void OctTree::insertInto(SceneNode* entity)
{
	glm::vec3 entityPos = entity->getPosition();

	sf::Vector3i index(static_cast<int>(entityPos.x / mXDiv)
						,static_cast<int>(entityPos.y / mYDiv)
						,static_cast<int>(entityPos.z / mZDiv));

	std::cout << index.x << ", " << index.y << ", " << index.z << std::endl;

//    mPartitions.at(index.x).at(index.y).at(index.z).insertEntity(entity);
}

