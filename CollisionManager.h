#pragma once

#include <vector>
#include <memory>
#include <SimpleMath.h>
#include <vector>
#include <functional>
#include <unordered_map>

#include "Collider.h"

class GameObject;
class SphereCollider;
class BoxCollider;


class CollisionManager final
{
	using ColliderList       = std::vector<Collider*>;
	using ColliderGroupList  = std::unordered_map<std::string, ColliderList>;
	using CollisionGroup     = std::pair<std::string, std::string>;
	using CollisionGroupList = std::list<CollisionGroup>;


	private:
		ColliderGroupList  m_colliderGroups;
		CollisionGroupList m_collisionGroups;

	public:
		CollisionManager();

	public:
		~CollisionManager();

	public:
		void Add(const std::string& groupName, Collider* collider);
		void AllowCollision(const std::string& groupName1, const std::string& groupName2);
		void DetectCollision();
		void DetectCollision(const ColliderList& colliders);
		void DetectCollision(const ColliderList& colliders1, const ColliderList& colliders2);


	public:
		static bool IsCollided(const SphereCollider* collider1, const SphereCollider* collider2);
		static bool IsCollided(const BoxCollider*    collider1, const BoxCollider*    collider2);
		static bool IsCollided(const SphereCollider* collider1, const BoxCollider*    collider2);
		static bool IsCollided(const BoxCollider*    collider1, const SphereCollider* collider2);
};
