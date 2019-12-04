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

Floor::Floor()
	: m_stage(nullptr), m_models{nullptr}, m_state(NONE)
{
}

void Floor::Initialize(Stage * stage, int x, int y)
{
	m_stage = stage;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
}

void Floor::Update(float elapsedTime)
{
}

void Floor::Render()
{
	if (!m_stage || !m_models[m_state]) return;

	// ���[���h�s��̍쐬
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);

	// �_���[�W���������u�Ԃ��烂�f���؂�ւ�����
	State state = m_state;

	auto gameWindow = GameContext::Get<GameWindow>();

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
}

void Floor::Reset()
{
	m_pos.y = 0.0f;
	if (m_state != Floor::NONE)
	{
		m_state = Floor::NORMAL;
	}
}

