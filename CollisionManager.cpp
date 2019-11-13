#include "pch.h"

#include "CollisionManager.h"

#include "GameObject.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Utilities.h"



CollisionManager::CollisionManager()
	: m_colliderGroups()
	, m_collisionGroups()
{
}



CollisionManager::~CollisionManager()
{
}



void CollisionManager::DetectCollision()
{
	for (const CollisionGroup& collisionGroup : m_collisionGroups)
	{
		if (collisionGroup.second.empty())
		{
			ColliderList& colliders = m_colliderGroups[collisionGroup.first];

			DetectCollision(colliders);
		}
		else
		{
			ColliderList& colliders1 = m_colliderGroups[collisionGroup.first];
			ColliderList& colliders2 = m_colliderGroups[collisionGroup.second];

			DetectCollision(colliders1, colliders2);
		}
	}

	m_colliderGroups.clear();
}



void CollisionManager::DetectCollision(const ColliderList& colliders)
{
	int numObjects = colliders.size();

	for (int i = 0; i < numObjects - 1; i++)
	{
		for (int j = i + 1; j < numObjects; j++)
		{
			if (colliders[i]->IsCollided(colliders[j]))
			{
				colliders[i]->OnCollision(colliders[j]);
				colliders[j]->OnCollision(colliders[i]);
			}
		}
	}
}



void CollisionManager::DetectCollision(const ColliderList& colliders1, const ColliderList& colliders2)
{
	for (const Collider* collider1 : colliders1)
	{
		for (const Collider* collider2 : colliders2)
		{
			if (collider1->IsCollided(collider2))
			{
				collider1->OnCollision(collider2);
				collider2->OnCollision(collider1);
			}
		}
	}
}



void CollisionManager::Add(const std::string& groupName, Collider* collider)
{
	m_colliderGroups[groupName].push_back(collider);
}



void CollisionManager::AllowCollision(const std::string& groupName1, const std::string& groupName2)
{
	if (groupName1 == groupName2)
	{
		m_collisionGroups.push_back(std::make_pair(groupName1, ""));
	}
	else
	{
		m_collisionGroups.push_back(std::make_pair(groupName1, groupName2));
	}
}



bool CollisionManager::IsCollided(const SphereCollider* collider1, const SphereCollider* collider2)
{
	return false;
}



bool CollisionManager::IsCollided(const BoxCollider* collider1, const BoxCollider* collider2)
{
	return false;
}



bool CollisionManager::IsCollided(const SphereCollider* collider1, const BoxCollider* collider2)
{
	return false;
}



bool CollisionManager::IsCollided(const BoxCollider* collider1, const SphereCollider* collider2)
{
	return IsCollided(collider2, collider1);
}