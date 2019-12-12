//======================================================
// File Name	: Parts.h
// Summary		: �v���C���[�N���X
// Date			: 2019/12/10
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "Object.h"

class GameWindow;

class Parts : public Object
{
public:
	// �p�[�c�̎��
	enum Kind
	{
		NONE,		// ���w��
		POWERUP,	// �p���[�A�b�v
		JUMP,		// �W�����v

		KIND_NUM
	};

	// �p�[�c�̏��
	enum STATE
	{
		STATE_NORMAL,	// �ʏ�
		STATE_FALL,		// ������
		STATE_DEAD,		// ���S
	};

	// ���Ƃ̔���p�̕��ƍ���
	static const float WIDTH;
	static const float HEIGHT;

private:
	// �p�[�c�̎��
	Kind m_kind;

	// �p�[�c�̏��
	STATE m_state;

public:
	// �R���X�g���N�^
	Parts();

	// �������֐�
	void Initialize(Kind kind, int x, int y, DirectX::Model* model);

	// �X�V�֐�
	void Update(float elapsedTime) override;

	// �I�u�W�F�N�g�h�c���擾����֐�
	Object::OBJECT_ID GetID() override;

	// �p�[�c�̎�ނ��擾����֐�
	Kind GetPartsKind() { return m_kind; }

	// �Փ˂�����Ă΂��֐�
	void OnCollision(GameObject* object);

	// ���Z�b�g�֐�
	void Reset();

	// �X�V�����i�������j
	void State_Fall(float elapsedTime);
};
