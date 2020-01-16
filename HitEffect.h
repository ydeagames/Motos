//======================================================
// File Name	: HitEffect.h
// Summary		: �Փˎ��̃G�t�F�N�g�\���N���X
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"

class GameWindow;

class HitEffect : public GameObject
{
	// �G�t�F�N�g�\������
	static const float DISPLAY_TIME;

	// �\���ʒu
	DirectX::SimpleMath::Vector3 m_pos;

	// �\�����ԃJ�E���^
	float m_timer;

public:
	// �R���X�g���N�^
	HitEffect();

	// �������֐�
	void Initialize(DirectX::SimpleMath::Vector3 pos);

	// �X�V�֐�
	void Update(float elapsedTime) override;

	// �`��֐�
	void Render() override;
};
