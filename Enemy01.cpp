//======================================================
// File Name	: Enemy01.cpp
// Summary		: �G�l�~�[
// Date			: 2019/12/18
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Enemy01.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Stage.h"
#include "SphereCollider.h"

// �d��
const float Enemy01::WEIGHT = 0.8f;
// ���Ƃ̔���p�̕��ƍ���
const float Enemy01::WIDTH = 0.2f;
const float Enemy01::HEIGHT = 0.2f;
// �I�u�W�F�N�g���m�̔���p�̔��a
const float Enemy01::RADIUS = 0.4f;
// �ő�ړ����x
const float Enemy01::MAX_SPEED = 0.06f;
// ���ɑ΂��門�C�W��
const float Enemy01::FRICTION = 0.05f;
// �v�l�Ԋu�i�P�ʁF�b�j
const float Enemy01::THINK_INTERVAL = 0.4f;

Enemy01::Enemy01(const std::string& tag)
	: Object(tag)
	, m_models{ nullptr }
	, m_state(STATE_NORMAL)
	, m_thinkTimer(0)
{
}

void Enemy01::Initialize(int x, int y)
{
	m_x = x;
	m_y = y;
	m_position = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);

	// ����
	m_weight = WEIGHT;

	// ���C�W��
	m_coefficientOfFriction = FRICTION;

	// ���a
	m_radius = RADIUS;

	// ���ƍ���
	m_width = Enemy01::WIDTH;
	m_height = Enemy01::HEIGHT;

	// �ʒu�ƌ��������Z�b�g
	Reset();

	// �����蔻��
	m_collider = std::make_unique<SphereCollider>(this, m_radius);
}

void Enemy01::SetModel(ModelType modelType, DirectX::Model* model)
{
	m_models[modelType] = model;
}

void Enemy01::Update(float elapsedTime)
{
	// �폜���N�G�X�g������΃^�X�N���폜
	if (IsInvalid()) return;

	// �A�N�e�B�u�t���O��false�̏ꍇ�͉������Ȃ�
	if (!m_activeFlag) return;

	// ��Ԃɂ���ď����𕪊򂳂���
	switch (m_state)
	{
	case STATE_NORMAL:	// �ʏ�
		State_Normal(elapsedTime);
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

	// ���C�ɂ�葬�x�𗎂Ƃ�
	Friction(elapsedTime);

	// �ő呬�x�ȏ�ɂȂ�Ȃ��悤����
	if (m_vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
	{
		m_vel.Normalize();
		m_vel *= MAX_SPEED;
	}

	// �ʒu�ɑ��x�𑫂�
	m_position += m_vel;

	// �v���C���[�Ə��̃`�F�b�N
	if (m_state == Enemy01::STATE_NORMAL && CheckFloor() == false)
	{
		// �������̂Œ�~������
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// ��Ԃ𗎉�����
		m_state = STATE_FALL;
		// �`�揇��FALL��
		SetDrawPrio(GameWindow::DRAW_FALL);
	}

	// �Փ˔���}�l�[�W���[�ɓo�^
	GameContext::Get<CollisionManager>()->Add(GetTag(), m_collider.get());
}

void Enemy01::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_models[NORMAL] || !m_displayFlag) return;

	// �����Ă���p�x���p�x�e�[�u������擾
	float angle = GameWindow::DIR_ANGLE[m_dir];

	// ���[���h�s����쐬
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(angle) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// ���f���̕`��i�W�����v�p�[�c��t���Ă��邩�ǂ����Ń��f�����Ⴄ�j
	m_models[NORMAL]->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

void Enemy01::State_Normal(float elapsedTime)
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	// �v���C���[���擾
	Object* target = gameWindow->GetStage()->GetPlayer();

	// ����I�Ƀv���C���[�̌�����ς���
	m_thinkTimer += elapsedTime;
	if (m_thinkTimer >= THINK_INTERVAL)
	{
		m_thinkTimer = 0.f;

		// �����_���ňړ�����������
		m_dir = rand() % 8;

		// �^�[�Q�b�g�����Ɍ�����
		if (target)
		{
			m_dir = GetDir(target);
		}
	}

	// �͂�������
	AddForce(GameWindow::DIR_ANGLE[m_dir], .03);

	// ���������Ȃ�����Ƒ��x�𔽓]����
	DirectX::SimpleMath::Vector3 tmp = m_position;
	m_position += m_vel;
	if (CheckFloor() == false)
	{
		DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180.f));
		m_vel = DirectX::SimpleMath::Vector3::Transform(m_vel, rotY);
		m_dir = (m_dir + 4) % 8;
	}
	m_position = tmp;
}

void Enemy01::State_Hit(float elapsedTime)
{
	// ���C�ɂ���~������
	if (m_vel == DirectX::SimpleMath::Vector3::Zero)
	{
		m_state = STATE_NORMAL;
	}
}

void Enemy01::State_Fall(float elapsedTime)
{
	m_position.y -= GameWindow::FALL_SPEED * elapsedTime;
	// ������x����������
	if (m_position.y < -GameWindow::FALL_DISTANCE)
	{
		// ���S
		m_state = STATE_DEAD;
		// �\���iOFF�j
		SetDisplayFlag(false);
	}
}

void Enemy01::OnCollision(GameObject* object)
{
	//if (Object * obj = dynamic_cast<Object*>(object))
	//{
	//	auto diff = GetPosition() - obj->GetPosition();
	//	auto angle = std::atan2(diff.z, diff.x);
	//	AddForce(angle, obj->GetHitForce());
	//}
	
	// �v�l�^�C�}�[��������
	m_thinkTimer = THINK_INTERVAL;
	// �Փ˂����̂ł��������~������
	m_vel = DirectX::SimpleMath::Vector3::Zero;
	// �Փ˂�������ւ̃x�N�g�������߂Ĉړ��������Z�o
	DirectX::SimpleMath::Vector3 v = object->GetPosition() - this->GetPosition();
	float angle = atan2f(v.x, v.z);
	// ���肩��󂯂�͂�������
	AddForce(angle, static_cast<Object*>(object)->GetHitForce());
	// �Փˏ�Ԃ�
	m_state = STATE_HIT;
}

Enemy01::STATE Enemy01::GetState()
{
	return m_state;
}

void Enemy01::Reset()
{
	// �v���C���[�����̏�Ԃɖ߂�
	m_dir = 0;
	m_position = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	m_state = STATE_NORMAL;
	SetDisplayFlag(true);
}
