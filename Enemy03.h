//======================================================
// File Name	: Enemy03.h
// Summary		: �v���C���[�N���X
// Date			: 2019/01/09
// Author		: YdeaGames
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"
#include "Enemy01.h"

class Enemy03 : public Enemy01
{
private:
	bool m_avoid;
	
public:
	Enemy03(const std::string& tag);
	
	// �������֐�
	void Initialize(int x, int y);

	int GetKeyToDir(int key);
	// �X�V�����i�ʏ�j
	void State_Normal(float elapsedTime) override;
};
