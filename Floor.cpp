//======================================================
// File Name	: Floor.cpp
// Summary		: ���N���X
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Floor.h"
#include "GameWindow.h"
#include "Stage.h"
#include <CommonStates.h>
#include "GameContext.h"
#include "DeviceResources.h"
#include "Camera.h"

// �_���[�W�̈ڍs����
const float Floor::DAMEGE_DELAY_TIME = 0.2f;

Floor::Floor()
	: m_stage(nullptr), m_models{nullptr}, m_state(NONE)
{
	SetDrawPrio(GameWindow::DRAW_STAGE);
}

void Floor::Initialize(Stage * stage, int x, int y)
{
	m_stage = stage;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
}

void Floor::Update(float elapsedTime)
{
	// �_���[�W�t���O�𗎂Ƃ�
	m_damageFlag = false;

	// �_���[�W�ڍs
	if (m_damageTimer > 0.0f)
	{
		m_damageTimer -= elapsedTime;
		if (m_damageTimer <= 0.0f)
		{
			m_damageTimer = 0.0f;
			switch (m_state)
			{
			case NORMAL:	// �ʏ�
				m_state = Floor::DAMAGED;	// �j����������
				break;
			case DAMAGED:	// �j��������
				m_state = Floor::FALL;		// ������
				break;
			default:
				break;
			}
		}
	}

	// ������Ԃ̏ꍇ�͏������ɗ��Ƃ�
	if (m_state == Floor::FALL)
	{
		m_pos.y -= 0.1f;

		// ������x�������������
		if (m_pos.y < -3.0f)
		{
			m_state = Floor::DEAD;
		}
	}
}

void Floor::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();

	if (!m_stage || !m_models[m_state]) return;

	// ���[���h�s��̍쐬
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);

	// �_���[�W���������u�Ԃ��烂�f���؂�ւ�����
	State state = m_state;


	// ���f���̕`��
	m_models[state]->Draw(GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()
		, *GameContext::Get<DirectX::CommonStates>()
		, world
		, gameWindow->GetCamera()->getViewMatrix()
		, gameWindow->GetCamera()->getProjectionMatrix(), false, [&]()
	{
		// ���͔������̉��Z�����i�������ňÂ��Ȃ��Ă��܂��̂Łj
		GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()->OMSetBlendState(GameContext::Get<DirectX::CommonStates>()->Additive(), nullptr, 0xffffffff);
	});

}

void Floor::SetModel(State state, DirectX::Model * model)
{
	m_models[state] = model;
}

void Floor::Damage()
{
	// �P�t���[���Ƀ_���[�W�͂P�x����
	if (m_damageFlag) return;
	m_damageFlag = true;
	// �_���[�W�ڍs�^�C�}�[�Z�b�g
	m_damageTimer = DAMEGE_DELAY_TIME;
}

void Floor::Reset()
{
	m_pos.y = 0.0f;
	if (m_state != Floor::NONE)
	{
		m_state = Floor::NORMAL;
		m_damageFlag = false;
		m_damageTimer = 0.0f;
	}
}

