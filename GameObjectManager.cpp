#include "pch.h"

#include "GameObjectManager.h"

#include "GameObject.h"



GameObjectManager::GameObjectManager()
	: m_objects()
	, m_objectQueue()
{
}



GameObjectManager::~GameObjectManager()
{
}



void GameObjectManager::Update(float elapsedTime)
{
	DestroyObjects();


	if (!m_objectQueue.empty())
	{
		AcceptObjects();
	}


	UpdateObjects(elapsedTime);
}



void GameObjectManager::Render()
{
	for (GameObjectPtr& object : m_objects)
	{
		object->Render();
	}
}



void GameObjectManager::Add(GameObjectPtr&& object)
{
	m_objectQueue.push_back(std::move(object));
}


GameObjectManager::GameObjectList& GameObjectManager::GetObjects()
{
	return m_objects;
}



void GameObjectManager::UpdateObjects(float elapsedTime)
{
	// ����1 �͈�for��
	for (GameObjectPtr& object : m_objects)
	{
		object->Update(elapsedTime);
	}


	// ����2 �C�e���[�^
	//GameObjectList::iterator it  = m_objects.begin();
	//GameObjectList::iterator end = m_objects.end();
	//while (it != end)
	//{
	//	(*it)->Update(elapsedTime);
	//	++it;
	//}


	// ����3 �����_��
	//std::for_each(
	//	m_objects.begin(),
	//	m_objects.end(),
	//	[&](GameObjectPtr& object) { object->Update(elapsedTime); }
	//);
}



void GameObjectManager::AcceptObjects()
{
	// ����1 �͈�for��
	//for (GameObjectPtr& object : m_objectQueue)
	//{
	//	m_objects.push_back(std::move(object));
	//}
	//m_objectQueue.clear();


	// ����2 �C�e���[�^
	//GameObjectList::iterator it  = m_objectQueue.begin();
	//GameObjectList::iterator end = m_objectQueue.end();
	//while (it != end)
	//{
	//	m_objects.push_back(std::move(*it));
	//	++it;
	//}
	//m_objectQueue.clear();


	// ����3 �v�f�̈ړ�
	m_objects.splice(m_objects.end(), m_objectQueue);
}



void GameObjectManager::DestroyObjects()
{
	// ����1 �C�e���[�^
	//GameObjectList::iterator it  = m_objects.begin();
	//while (it != m_objects.end())
	//{
	//	if ((*it)->IsInvalid())
	//	{
	//		it = m_objects.erase(it);
	//	}
	//	else
	//	{
	//		++it;
	//	}
	//}


	// ����2 ������v����S�Ă̗v�f���폜
	m_objects.remove_if(std::mem_fn(&GameObject::IsInvalid));


	// ����3 �����_��
	//m_objects.remove_if([](const GameObjectPtr& object) { return object->IsInvalid(); });
}
