#pragma once

#include "Common.h"

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>>	EntityVec;
typedef std::map<entityTags, EntityVec>		EntityMap;

class EntityManager
{
private:
	EntityVec m_entities;
	EntityMap m_entityMap;
	EntityVec m_toAdd;
	size_t    m_totalEntities{ 0 };

	void removeDeadEntities(EntityVec& vec);

public:

	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const entityTags tag);

	const EntityVec& getEntities();
	const EntityVec& getEntities(const entityTags tag);
};

