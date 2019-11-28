//======================================================
// File Name	: Bg.h
// Summary		: �w�i�N���X
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include <Model.h>

class GameWindow;

class Bg : public GameObject
{
	// �Q�[���E�C���h�E�ւ̃|�C���^
	GameWindow* m_gameWindow;

	// ���f���f�[�^�ւ̃|�C���^
	std::unique_ptr<DirectX::Model> m_model;

	// ��]�p
	float m_angle;

public:
	// �R���X�g���N�^
	Bg();

	// �������֐�
	void Initialize(GameWindow* gameWindow);

	// �X�V
	virtual void Update(float elapsedTime);
	// �`��֐�
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);
};