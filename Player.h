//======================================================
// File Name	: Player.h
// Summary		: �v���C���[�N���X
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"
#include "SphereCollider.h"

class Player : public Object
{
public:
	// �v���C���[�̃^�C�v�ʃ��f��
	enum ModelType
	{
		NORMAL,	// �ʏ�
		WING,	// �E�C���O�t��

		MODEL_TYPE_NUM
	};

	// �v���C���[�̏��
	enum STATE
	{
		STATE_NORMAL,	// �ʏ�
		STATE_JUMP,		// �W�����v��
		STATE_HIT,		// ������΂�����
		STATE_FALL,		// ������
		STATE_DEAD,		// ���S
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

	// �W�����v���Ă���t���[����
	static const int JUMP_FRAME;

	// �W�����v�̍���
	static const float JUMP_HEIGHT;


private:
	// ���f���f�[�^�ւ̃|�C���^
	DirectX::Model* m_models[MODEL_TYPE_NUM];

	// �v���C���[�̏��
	STATE m_state;

	// �p���[�A�b�v
	int m_powerupParts;

	// �W�����v�p�[�c�𑕒����Ă��邩�H
	bool m_jumpParts;

	// �W�����v�J�E���^�[
	int m_jumpCounter;

	// �������̉�]
	float m_fallRotateAngle;

	// �W�����v�I�����ɌĂ΂��֐�
	std::function<void(Object*)> m_jumpEndFunction;

	// �R���C�_�[
	std::unique_ptr<SphereCollider> m_collider;

	//
	DirectX::SimpleMath::Vector2 m_lastPos;


public:
	// �R���X�g���N�^
	Player(const std::string& tag, DirectX::Model* shadowModel);

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

	// �X�V�����i�W�����v���j
	void State_Jump(float elapsedTime);

	// �X�V�����i�Փ˂��Đ���s�\���j
	void State_Hit(float elapsedTime);

	// �X�V�����i�������j
	void State_Fall(float elapsedTime);

	// �Փ˂�����Ă΂��֐�
	void OnCollision(GameObject* object);
	void OnCollision_Enemy01(GameObject* object);

	// �v���C���[�̏�Ԃ��擾����֐�
	Player::STATE GetState();

	// �v���C���[�̈ړ��֐�
	void Move(float elapsedTime, const DirectX::Keyboard::KeyboardStateTracker& tracker);

	// ���Z�b�g�֐�
	void Reset();

	// �p���[�A�b�v�̐ݒ�֐�
	void SetPowerupParts(int cnt);

	// �W�����v�p�[�c�����̐ݒ�֐�
	void SetJumpParts(bool flag);

	// �W�����v�I�����ɌĂ΂��֐���ݒ肷��֐�
	void SetJumpEndFunction(std::function<void(Object*)> func);


private:
	// �L�[���͂�����������߂�֐�
	int GetKeyToDir(int key);
};
