//======================================================
// File Name	: ObjectManager.cpp
// Summary		: �Q�[����ʂƏ���ʂ̃I�u�W�F�N�g�}�l�[�W���[
//				�@���܂Ƃ߂��N���X
// Date			: 2019/11/12
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "ObjectManager.h"

#include "GameObjectManager.h"

ObjectManager::ObjectManager()
{
	// �Q�[����ʗp�I�u�W�F�N�g�}�l�[�W���[����
	m_pGameOM = std::make_unique<GameObjectManager>();
	// ����ʗp�I�u�W�F�N�g�}�l�[�W���[����
	m_pInfoOM = std::make_unique<GameObjectManager>();
}


ObjectManager::~ObjectManager()
{
}
