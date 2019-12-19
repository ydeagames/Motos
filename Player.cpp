//======================================================
// File Name	: Player.cpp
// Summary		: �v���C���[�N���X
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Player.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"

// �v���C���[�̏d��
const float Player::WEIGHT = 1.0f;

// ���ɑ΂��門�C�W��
const float Player::FRICTION = 0.1f;

// ���Ƃ̔���p�̕��ƍ���
const float Player::WIDTH = 0.2f;
const float Player::HEIGHT = 0.2f;

// �v���C���[�̍ő�ړ����x
const float Player::MAX_SPEED =	0.1f;

// �W�����v���Ă���t���[����
const int Player::JUMP_FRAME = 30;

// �W�����v�̍���
const float Player::JUMP_HEIGHT = 1.5f;

float SLerp(float start, float end, float t)
{
	float rate = (sinf(3.f * DirectX::XM_PI / 2.f + DirectX::XM_PI * t) + 1.f) / 2.f;
	return start + (end - start) * rate;
}

Player::Player()
	: m_models{ nullptr }
	, m_powerupParts(0)
	, m_jumpParts(false)
	, m_state(STATE_NORMAL)
	, m_jumpCounter(0)
	, m_fallRotateAngle(0.0f)
{
}

void Player::Initialize(int x, int y)
{
	m_x = x;
	m_y = y;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);

	// ���ʂ�ݒ�
	SetPowerupParts(0);

	// ���C�W��
	m_coefficientOfFriction = FRICTION;

	// ���ƍ���
	m_weight = Player::WIDTH;
	m_height = Player::HEIGHT;
}

void Player::SetModel(ModelType modelType, DirectX::Model * model)
{
	m_models[modelType] = model;
}

void Player::Update(float elapsedTime)
{
	// �폜���N�G�X�g������΃^�X�N���폜
	if (IsInvalid()) return;

	// �A�N�e�B�u�t���O��false�̏ꍇ�͉������Ȃ�
	if (!m_activeFlag) return;

	// ���C�ɂ�葬�x�𗎂Ƃ�
	Friction(elapsedTime);

	// �ő呬�x�ȏ�ɂȂ�Ȃ��悤����
	if (m_vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
	{
		m_vel.Normalize();
		m_vel *= MAX_SPEED;
	}

	// ��Ԃɂ���ď����𕪊򂳂���
	switch (m_state)
	{
	case STATE_NORMAL:	// �ʏ�
		break;
	case STATE_JUMP:	// �W�����v��
		State_Jump(elapsedTime);
		break;
	case STATE_HIT:		// �q�b�g��
		State_Hit(elapsedTime);
		break;
	case STATE_FALL:	// ������
		State_Fall(elapsedTime);
		break;
	default:
		break;
	}

	// �ʒu�ɑ��x�𑫂�
	m_pos += m_vel;

	// �v���C���[�Ə��̃`�F�b�N
	if (m_state == Player::STATE_NORMAL && CheckFloor() == false)
	{
		// �������̂Œ�~������
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// ��Ԃ𗎉�����
		m_state = STATE_FALL;
		// �`�揇��FALL��
		SetDrawPrio(GameWindow::DRAW_FALL);
	}

	// �W�����v�̏���
	if (m_jumpCounter != 0)
	{
		// �Q�[�����ۂ��W�����v�ɂ��邽�߁A�T�C���g�ŃW�����v���̍������Z�o����
		m_jumpCounter--;
		int cnt = JUMP_FRAME - m_jumpCounter;
		float angle = 180.0f / (float)JUMP_FRAME * cnt;
		m_pos.y = sinf(DirectX::XMConvertToRadians(angle)) * JUMP_HEIGHT;
		if (m_jumpCounter == 0)
		{
			// ���ɒ��n����
			m_pos.y = 0.0f;
			// ���Ƀ_���[�W��^����
			if (m_jumpEndFunction) m_jumpEndFunction(this);
		}
	}
}

void Player::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_models[NORMAL] || !m_displayFlag) return;

	// �����Ă���p�x���p�x�e�[�u������擾
	float angle = GameWindow::DIR_ANGLE[m_dir];

	// �������̉�]��������
	angle += m_fallRotateAngle;

	// ���[���h�s����쐬
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(angle) * DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);

	// ���f���̕`��i�W�����v�p�[�c��t���Ă��邩�ǂ����Ń��f�����Ⴄ�j
	m_models[m_jumpParts ? WING : NORMAL]->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

void Player::State_Jump(float elapsedTime)
{
	if (m_jumpCounter == 0)
	{
		// �ʏ�̏�Ԃ�
		m_state = STATE_NORMAL;
	}
}

void Player::State_Hit(float elapsedTime)
{
	// ���C�ɂ���~������
	if (m_vel == DirectX::SimpleMath::Vector3::Zero)
	{
		if (m_jumpCounter != 0)
		{
			m_state = STATE_JUMP;
		}
		else
		{
			m_state = STATE_NORMAL;
		}
	}
}

void Player::State_Fall(float elapsedTime)
{
	m_pos.y -= GameWindow::FALL_SPEED * elapsedTime;
	// ��]���Ȃ��痎������
	m_fallRotateAngle -= DirectX::XMConvertToRadians(GameWindow::FALL_ROTATE_SPEED) * elapsedTime;
	// ������x����������
	if (m_pos.y < -GameWindow::FALL_DISTANCE)
	{
		// ���S
		m_state = STATE_DEAD;
		// �\���iOFF�j
		SetDisplayFlag(false);
		// �������̉�]�����Z�b�g
		m_fallRotateAngle = 0.0f;
	}
}

void Player::OnCollision(GameObject* object)
{
}

Player::STATE Player::GetState()
{
	return m_state;
}

void Player::Move(float elapsedTime, const DirectX::Keyboard::KeyboardStateTracker& tracker)
{
	// �v���C���[���ʏ�A�W�����v���݈̂ړ�����
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP) return;

	// �L�[���͂��擾
	auto kb = DirectX::Keyboard::Get().GetState();

	// �������
	float force = 0.0f;

	// �����L�[�������ꂽ��
	if (kb.Up || kb.Down || kb.Left || kb.Right)
	{
		// �����ꂽ�����L�[�̃r�b�g�𗧂Ă�
		int key = 0;
		if (kb.Up) key |= 1 << GameWindow::UP;
		if (kb.Down) key |= 1 << GameWindow::DOWN;
		if (kb.Left) key |= 1 << GameWindow::LEFT;
		if (kb.Right) key |= 1 << GameWindow::RIGHT;
		int dir = GetKeyToDir(key);
		if (dir != -1) m_dir = dir;

		// �͂������Ď��@������������
		force = 0.03f;
	}

	// �X�y�[�X�L�[�ŃW�����v
	if (m_state == STATE_NORMAL && m_jumpParts && tracker.pressed.Space)
	{
		m_state = STATE_JUMP;
		m_jumpCounter = JUMP_FRAME;
	}

	// �͂�������
	AddForce(GameWindow::DIR_ANGLE[m_dir], force);
}

void Player::Reset()
{
	// �v���C���[�����̏�Ԃɖ߂�
	m_dir = 0;
	m_pos = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	m_state = STATE_NORMAL;
	SetDisplayFlag(true);
}

void Player::SetPowerupParts(int cnt)
{
	m_powerupParts = cnt;
	m_weight = WEIGHT + m_powerupParts * 0.1f;
}

void Player::SetJumpParts(bool flag)
{
	m_jumpParts = flag;
}

int Player::GetKeyToDir(int key)
{
	// �r�b�g�t�B�[���h�̃L�[�����W����(0�`7�j�ɕϊ�����e�[�u��
	int table[16] = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5, -1, -1, -1 };
	return table[key];
}

void Player::SetJumpEndFunction(std::function<void(Object*)> func)
{
	// �W�����v�̏I�����ɌĂ΂��֐���o�^����
	m_jumpEndFunction = func;
}
