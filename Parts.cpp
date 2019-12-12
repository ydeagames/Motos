//======================================================
// File Name	: Parts.cpp
// Summary		: �v���C���[�N���X
// Date			: 2019/12/10
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Parts.h"
#include "GameWindow.h"

// ���Ƃ̔���p�̕��ƍ���
const float Parts::WIDTH = 0.2f;
const float Parts::HEIGHT = 0.2f;

Parts::Parts()
	: m_kind(Kind::NONE), m_state(STATE_NORMAL)
{
}

void Parts::Initialize(Kind kind, int x, int y, DirectX::Model * model)
{
	m_model = model;
	m_x = x;
	m_y = y;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
	m_kind = kind;

	// ����
	m_weight = 0.1f;

	// ���C�W��
	m_coefficientOfFriction = 5.0f;

	// ���a
	m_radius = 0.4f;

	// ���ƍ���
	m_width = Parts::WIDTH;
	m_height = Parts::HEIGHT;
}

void Parts::Update(float elapsedTime)
{
	// ������������������ �L�q���Ă݂悤 ������������������

	// �폜���N�G�X�g������΃^�X�N���폜
	if (IsInvalid()) return;

	// ���C�ɂ�葬�x�𗎂Ƃ�
	Friction(elapsedTime);

	// ��Ԃɂ���ď����𕪊򂳂���
	switch (m_state)
	{
	case STATE_NORMAL:	// �ʏ�
		break;
	case STATE_FALL:	// ������
		State_Fall(elapsedTime);
		break;
	default:
		break;
	}

	// �ʒu�ɑ��x�𑫂�
	m_pos += m_vel;

	// �ʏ��Ԃŏ��̏�łȂ���Η�������
	if (m_state == STATE_NORMAL && CheckFloor() == false)
	{
		// �������̂Œ�~������
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// ��Ԃ𗎉�����
		m_state = STATE_FALL;
		// �`�揇��ς���
		SetDrawPrio(GameWindow::DRAW_FALL);
	}

	// ������������������ �L�q���Ă݂悤 ������������������
}

Object::OBJECT_ID Parts::GetID()
{
	if (m_kind == POWERUP) return Object::POWERUP_PARTS;
	if (m_kind == JUMP) return Object::JUMP_PARTS;
	return Object::NONE;
}

void Parts::OnCollision(GameObject* object)
{
}

void Parts::Reset()
{
	// �Ăю���ĂȂ���Ԃɖ߂�
	SetActive(true);
	SetDisplayFlag(true);
	m_state = STATE_NORMAL;
	m_dir = 0;
	m_pos = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	SetDrawPrio(GameWindow::DRAW_OBJECT);
}

void Parts::State_Fall(float elapsedTime)
{
	// ������������������ �L�q���Ă݂悤 ������������������

	// ���ɗ�����
	m_pos.y -= GameWindow::FALL_SPEED * elapsedTime;

	// ������x����������
	if (m_pos.y < -GameWindow::FALL_DISTANCE)
	{
		// �X�e�[�g�ύX
		m_state = STATE_DEAD;
		// �\���iOFF�j
		SetDisplayFlag(false);
	}
	// ������������������ �L�q���Ă݂悤 ������������������
}
