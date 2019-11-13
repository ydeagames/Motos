//======================================================
// File Name	: ObjectManager.h
// Summary		: �Q�[����ʂƏ���ʂ̃I�u�W�F�N�g�}�l�[�W���[
//				�@���܂Ƃ߂��N���X
// Date			: 2019/11/12
// Author		: Takafumi Ban
//======================================================
#pragma once

class GameObjectManager;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

public:	// �A�N�Z�b�T
		// �Q�[����ʂ̃I�u�W�F�N�g�}�l�[�W���[�擾
	GameObjectManager* GetGameOM()
	{
		return m_pGameOM.get();
	}
	// ����ʂ̃I�u�W�F�N�g�}�l�[�W���[�擾
	GameObjectManager* GetInfoOM()
	{
		return m_pInfoOM.get();
	}

private:
	// �Q�[���I�u�W�F�N�g�}�l�[�W���[�i�Q�[����ʁj
	std::unique_ptr<GameObjectManager>      m_pGameOM;
	// �Q�[���I�u�W�F�N�g�}�l�[�W���[�i����ʁj
	std::unique_ptr<GameObjectManager>      m_pInfoOM;
};

