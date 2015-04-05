#include "OctTreePartition.hpp"

#include <algorithm>
#include <cassert>

void OctTreePartition::insertEntity(SceneNode* entity)
{
    mEntities.push_back(entity);
}

void OctTreePartition::deleteEntity(SceneNode* entity)
{
    auto it = find(mEntities.begin(), mEntities.end(), entity);

    assert(it != mEntities.end());

    mEntities.erase(it);
}
