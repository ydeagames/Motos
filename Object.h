//======================================================
// File Name	: Object.h
// Summary		: �I�u�W�F�N�g�N���X
// Date			: 2019/12/02
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include <Model.h>

class GameWindow;
class Shadow;

// �I�u�W�F�N�g�N���X
class Object : public GameObject
{
public:

	// �I�u�W�F�N�g�h�c
	enum OBJECT_ID
	{
		NONE,			// �Ȃ�
		PLAYER,			// �v���C���[
		POWERUP_PARTS,	// �p���[�A�b�v�p�[�c
		JUMP_PARTS,		// �W�����v�p�[�c
		ENEMY_1,		// �G�P
		ENEMY_2,		// �G�Q
	};

public:
	// �R���X�g���N�^
	Object(const std::string& tag, DirectX::Model* shadowModel = nullptr);

	// �X�V
	void Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �I�u�W�F�N�g�h�c���擾����֐�
	virtual Object::OBJECT_ID GetID() = 0;

	// ���a���擾����֐�
	float GetRadius() { return m_radius; }

	// �Փ˃t���O�̐ݒ�֐�
	void SetHitFlag(bool flag) { m_hitFlag = flag; }

	// �Փ˃t���O�̎擾�֐�
	bool GetHitFlag() { return m_hitFlag; }

	// �����擾����֐�
	float GetWidth() { return m_width; }

	// �������擾����֐�
	float GetHeight() { return m_height; }

	// ���ʂ��擾����֐�
	float GetWeight() { return m_weight; }

	// ���̃`�F�b�N�֐��̓o�^�֐�
	void SetCheckFloorFunction(std::function<bool(Object* object)> func);

	// ���_�����Z����֐��̓o�^�֐�
	void SetAddScoreFunction(std::function<void(int score)> func);

	// �\����ON/OFF����֐�
	void SetDisplayFlag(bool flag) { m_displayFlag = flag; }

	// �\����ON/OFF���擾����֐�
	bool GetDisplayFlag() { return m_displayFlag; }

	// �X�V�֐������s���邩�ǂ����ݒ肷��֐�
	void SetActive(bool flag) { m_activeFlag = flag; }

	const DirectX::SimpleMath::Vector3& GetVelocity() const;
	
	const DirectX::SimpleMath::Vector3& GetAcceleration() const;

	// �Փ˂������ɑ���ɗ^����͂��擾����֐�
	virtual float GetHitForce();

	// ���Z�b�g�֐�
	virtual void Reset() {}

	// �e�̕\���� ON/OFF ����֐�
	void ShadowActive(bool flag);

protected:
	// �͂�������֐�
	void AddForce(float angle, float force);

	// ���C�ɂ�葬�x����������֐�
	void Friction(float elapsedTime);

	// �^�[�Q�b�g�̕�����Ԃ��֐��i�^����O�Ƃ��āA�t���v��]�łW�����j
	int GetDir(Object* object);

	// ���̃`�F�b�N�֐��ifalse�F���Ȃ��j
	bool CheckFloor();

	// ���_�����Z����֐�
	void AddScore(int score);

protected:

	// ���f���f�[�^�ւ̃|�C���^
	DirectX::Model* m_model;

	// �����Ă�������i�W�����ŕ\���B�e�r�b�g�ŉ����ꂽ������\���B�j
	int m_dir;

	// ����
	float m_weight;

	// ���Ƃ̖��C�W���i�ʁj
	float m_coefficientOfFriction;

	// �����x
	DirectX::SimpleMath::Vector3 m_acc;

	// ���x
	DirectX::SimpleMath::Vector3 m_vel;

	// ���a
	float m_radius;

	// ���ƍ���
	float m_width, m_height;

	// �Փ˃t���O
	bool m_hitFlag;

	// ���̃`�F�b�N�֐�
	std::function<bool(Object*)> m_floorCheckFunction;

	// ���_�����Z����֐�
	std::function<void(int score)> m_addScoreFunction;

	// �\���t���O
	bool m_displayFlag;

	// �X�V�֐������s���邩�ǂ������f����t���O
	bool m_activeFlag;

	// �X�e�[�W��̈ʒu
	int m_x, m_y;

	// �e
	Shadow* m_shadow;

public:
	static DirectX::SimpleMath::Vector2 GetXZ(const DirectX::SimpleMath::Vector3& vec)
	{
		return DirectX::SimpleMath::Vector2(vec.x, vec.z);
	}
};
