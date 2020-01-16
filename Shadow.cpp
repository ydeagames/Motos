//======================================================
// File Name	: Shadow.cpp
// Summary		: �e�N���X�iObject�ɕt����e�j
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Shadow.h"
#include "Game.h"
#include "GameWindow.h"
#include "Object.h"
#include "GameContext.h"
#include "Camera.h"

Shadow::Shadow()
	: m_active(true)
{
	// �`�揇�̐ݒ�
	SetDrawPrio(GameWindow::DRAW_SHADOW);
}

void Shadow::Initialize(Object* pParent, DirectX::Model * model)
{
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();

	m_pParent = pParent;
	m_model = model;

	// �u�����h�X�e�[�g�쐬�i���Z�����j
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	deviceResources->GetD3DDevice()->CreateBlendState(&bd, m_blendState.GetAddressOf());
}

void Shadow::Update(float elapsedTime)
{
}

void Shadow::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	DirectX::CommonStates* state = GameContext::Get<DirectX::CommonStates>();

	if (!gameWindow || !m_model || !m_active || !m_pParent) return;

	// �e�̐^���ɉe��\������
	const DirectX::SimpleMath::Vector3& pos = m_pParent->GetPosition();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(pos.x, 0.0f, pos.z));
	m_model->Draw(deviceResources->GetD3DDeviceContext(), *state
			, world
			, gameWindow->GetCamera()->getViewMatrix()
			, gameWindow->GetCamera()->getProjectionMatrix()
		, false, [&]()
	{
		// �u�����h�����Z�����ɂ��[�x�o�b�t�@�͎g�p�����`�悷��
		deviceResources->GetD3DDeviceContext()->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
		deviceResources->GetD3DDeviceContext()->OMSetDepthStencilState(state->DepthNone(), 0);
	});
}
