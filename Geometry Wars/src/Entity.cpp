#include "Entity.h"

Entity::Entity(size_t id, const entityTags tag)
	: m_id(id),
	m_tag(tag)
{
}

void Entity::destroy()
{
	m_active = false;
}

bool Entity::isActive() const
{
	return m_active;
}

const entityTags Entity::tag() const
{
	return m_tag;
}

size_t Entity::id() const
{
	return m_id;
}