//======================================================
// File Name	: Enemy01.h
// Summary		: �v���C���[�N���X
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"
#include "SphereCollider.h"

class Enemy01 : public Object
{
public:
	// �v���C���[�̃^�C�v�ʃ��f��
	enum ModelType
	{
		ENEMY1,
		ENEMY2,
		ENEMY3,

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


protected:
	// ���f���f�[�^�ւ̃|�C���^
	DirectX::Model* m_models[MODEL_TYPE_NUM];

	// �v���C���[�̏��
	STATE m_state;

	// �v�l�^�C�}�[
	float m_thinkTimer;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> m_collider;


public:
	// �R���X�g���N�^
	Enemy01(const std::string& tag, DirectX::Model* shadowModel);

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
	virtual void State_Normal(float elapsedTime);

	// �X�V�����i�Փ˂��Đ���s�\���j
	virtual void State_Hit(float elapsedTime);

	// �X�V�����i�������j
	virtual void State_Fall(float elapsedTime);

	// �Փ˂�����Ă΂��֐�
	void OnCollision(GameObject* object);

	// �v���C���[�̏�Ԃ��擾����֐�
	Enemy01::STATE GetState();

	// ���Z�b�g�֐�
	void Reset();
};
