//======================================================
// File Name	: Enemy01.h
// Summary		: �v���C���[�N���X
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"

class Enemy01 : public Object
{
public:
	// �v���C���[�̃^�C�v�ʃ��f��
	enum ModelType
	{
		NORMAL,	// �ʏ�

		MODEL_TYPE_NUM
	};

	// �G�̏��
	enum STATE
	{
		STATE_NORMAL, // �ʏ�
		STATE_HIT, // ������΂�����
		STATE_FALL, // ������
		STATE_DEAD, // ���S
	};

	// ���Ƃ̔���p�̕��ƍ���
	static const float WIDTH;
	static const float HEIGHT;

	static const float RADIUS;
	
	// �v���C���[�̍ő�ړ����x
	static const float MAX_SPEED;

	// �v���C���[�̏d��
	static const float WEIGHT;

	// ���ɑ΂��門�C�W��
	static const float FRICTION;

	// �v�l�Ԋu�i�P�ʁF�b�j
	static const float THINK_INTERVAL;


private:
	// ���f���f�[�^�ւ̃|�C���^
	DirectX::Model* m_models[MODEL_TYPE_NUM];

	// �v���C���[�̏��
	STATE m_state;

	// �v�l�^�C�}�[
	float m_thinkTimer;


public:
	// �R���X�g���N�^
	Enemy01();

	// �������֐�
	void Initialize(int x, int y);

	// ���f���ݒ�֐�
	void SetModel(ModelType modelType, DirectX::Model* model);

	// �I�u�W�F�N�g�h�c���擾����֐�
	Object::OBJECT_ID GetID() { return Object::PLAYER; }

	// �X�V�֐�
	void Update(float elapsedTime) override;

	// �`��֐�
	void Render() override;

	// �X�V�����i�ʏ�j
	void State_Normal(float elapsedTime);
	// �X�V�����i�Փ˂��Đ���s�\���j
	void State_Hit(float elapsedTime);

	// �X�V�����i�������j
	void State_Fall(float elapsedTime);

	// �Փ˂�����Ă΂��֐�
	void OnCollision(GameObject* object);

	// �v���C���[�̏�Ԃ��擾����֐�
	Enemy01::STATE GetState();

	// �v���C���[�̈ړ��֐�
	void Move(float elapsedTime, const DirectX::Keyboard::KeyboardStateTracker& tracker);

	// ���Z�b�g�֐�
	void Reset();
};
