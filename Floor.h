//======================================================
// File Name	: Floor.h
// Summary		: ���N���X
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include <Model.h>

class Stage;

class Floor : public GameObject
{
public:
	// ���̏��
	enum State
	{
		NONE,		// ���Ȃ�
		NORMAL,		// �ʏ�
		DAMAGED,	// �j��������
		FALL,		// ������
		DEAD,		// �������ď�����

		STATE_NUM
	};

private:
	// �X�e�[�W�ւ̃|�C���^
	Stage* m_stage;

	// ���f���f�[�^�ւ̃|�C���^
	DirectX::Model* m_models[STATE_NUM];

	// ���̏��
	State m_state;

	// �ʒu
	DirectX::SimpleMath::Vector3 m_pos;

public:
	// �R���X�g���N�^
	Floor();

	// �������֐�
	void Initialize(Stage* stage, int x, int y);

	// �X�V
	virtual void Update(float elapsedTime);
	// �`��֐�
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);

	// ���f���ݒ�֐�
	void SetModel(State state, DirectX::Model* model);

	// ���̏�Ԃ�ݒ肷��֐�
	void SetState(Floor::State state) { m_state = state; }

	// ���̏�Ԃ��擾����֐�
	Floor::State GetState() { return m_state; }

	// �_���[�W��^����֐�
	void Damage();

	// ���Z�b�g�֐�
	void Reset();
};