//======================================================
// File Name	: SmokeEffect.h
// Summary		: ���̃G�t�F�N�g�\���N���X
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"

class GameWindow;

class SmokeEffect : public GameObject
{
private:
	// �G�t�F�N�g�\������
	static const float DISPLAY_TIME;

	// �\���ʒu
	DirectX::SimpleMath::Vector3 m_pos;

	// �\�����ԃJ�E���^
	float m_timer;

	// �A���t�@�l
	float m_alpha;

	// �ړ����x
	DirectX::SimpleMath::Vector3 m_v;

public:
	// �R���X�g���N�^
	SmokeEffect();
	
	// �������֐�
	void Initialize(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 v);

	// �X�V�֐�
	void Update(float elapsedTime) override;

	// �`��֐�
	void Render() override;
};