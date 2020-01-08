//======================================================
// File Name	: Object.cpp
// Summary		: �I�u�W�F�N�g�N���X
// Date			: 2019/12/02
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Object.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "GameContext.h"
#include "Camera.h"

Object::Object(const std::string& tag)
	: GameObject(tag)
	, m_model(nullptr)
	, m_dir(0)
	, m_weight(0.0f)
	, m_coefficientOfFriction(0.0f)
	, m_radius(0.0f)
	, m_width(0.0f)
	, m_height(0.0f)
	, m_hitFlag(false)
	, m_displayFlag(true)
	, m_activeFlag(true)
	, m_x(0)
	, m_y(0)
{
	SetDrawPrio(GameWindow::DRAW_OBJECT);
}

void Object::Update(float elapsedTime)
{
	if (!m_activeFlag) return;

	// �ʒu�ɑ��x�𑫂�
	m_position += m_vel;
}

void Object::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_model || !m_displayFlag) return;
	
	// ���[���h�s����쐬
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// ���f���̕`��
	m_model->Draw(deviceResources->GetD3DDeviceContext(), *GameContext::Get<DirectX::CommonStates>()
		, world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

const DirectX::SimpleMath::Vector3& Object::GetVelocity() const
{
	return m_vel;
}

const DirectX::SimpleMath::Vector3& Object::GetAcceleration() const
{
	return m_acc;
}

float Object::GetHitForce()
{
	float force = 0.4f;

	// �A���q�b�g���̑��x�̉e�����傫�������̂ő��x��}����
	float v = m_vel.Length();
	if (v >= 0.1f) v = 0.1f;
	force += m_weight * v;

	return force;
}

void Object::AddForce(float angle, float force)
{
	// �i�s�����x�N�g��
	DirectX::SimpleMath::Vector3 dir(0.0f, 0.0f, -1.0f);

	// ���@�̌����x�N�g�������߂�
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(angle);
	dir = DirectX::SimpleMath::Vector3::Transform(dir, rotY);

	// �����x
	m_acc = dir * (force / m_weight);

	// ���x�ɉ����x�𑫂�
	m_vel += m_acc;
}

void Object::Friction(float elapsedTime)
{
	// ���C�� = ���C�W�� �~ �d�͉����x �~ ����
	float frictionForce = m_coefficientOfFriction * m_weight * GameWindow::GRAVITY;

	// ���C�͂ɂ���đ��x�𗎂Ƃ�
	float v = (frictionForce / m_weight) * elapsedTime;

	if (m_vel.LengthSquared() > v * v)
	{
		v = m_vel.Length() - v;
		m_vel.Normalize();
		m_vel *= v;
	}
	else
	{
		m_vel = DirectX::SimpleMath::Vector3::Zero;
	}
}

int Object::GetDir(Object * object)
{
	// �^�[�Q�b�g�ւ̃x�N�g�������߂�
	DirectX::SimpleMath::Vector3 v = object->GetPosition() - m_position;

	// ���ς��g����ԋ߂��p�x�����߂�
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(45.0f));
	DirectX::SimpleMath::Vector3 dirVec(0.0f, 0.0f, -1.0f);
	int dir = 0;
	float maxVal = dirVec.Dot(v);
	// ���ς���ԑ傫���������߂�
	for (int i = 0; i < 7; i++)
	{
		dirVec = DirectX::SimpleMath::Vector3::Transform(dirVec, rotY);
		float val = dirVec.Dot(v);
		if (val > maxVal)
		{
			dir = i + 1;
			maxVal = val;
		}
	}
	return dir;
}

bool Object::CheckFloor()
{
	if (m_floorCheckFunction)
	{
		return m_floorCheckFunction(this);
	}
	return true;
}

void Object::AddScore(int score)
{
	if (m_addScoreFunction)
	{
		m_addScoreFunction(score);
	}
}

void Object::SetCheckFloorFunction(std::function<bool(Object*)> func)
{
	m_floorCheckFunction = func;
}

void Object::SetAddScoreFunction(std::function<void(int score)> func)
{
	m_addScoreFunction = func;
}

