//======================================================
// File Name	: Enemy02.h
// Summary		: �v���C���[�N���X
// Date			: 2019/12/19
// Author		: YdeaGames
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"
#include "Enemy01.h"

class Enemy02 : public Enemy01
{
public:
	// �������֐�
	void Initialize(int x, int y);

	// �X�V�����i�ʏ�j
	void State_Normal(float elapsedTime) override;
};
