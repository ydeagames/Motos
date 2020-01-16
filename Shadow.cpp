//======================================================
// File Name	: Shadow.cpp
// Summary		: 影クラス（Objectに付ける影）
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
	// 描画順の設定
	SetDrawPrio(GameWindow::DRAW_SHADOW);
}

void Shadow::Initialize(Object* pParent, DirectX::Model * model)
{
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();

	m_pParent = pParent;
	m_model = model;

	// ブレンドステート作成（減算合成）
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

	// 親の真下に影を表示する
	const DirectX::SimpleMath::Vector3& pos = m_pParent->GetPosition();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(pos.x, 0.0f, pos.z));
	m_model->Draw(deviceResources->GetD3DDeviceContext(), *state
			, world
			, gameWindow->GetCamera()->getViewMatrix()
			, gameWindow->GetCamera()->getProjectionMatrix()
		, false, [&]()
	{
		// ブレンドを減算合成にし深度バッファは使用せず描画する
		deviceResources->GetD3DDeviceContext()->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
		deviceResources->GetD3DDeviceContext()->OMSetDepthStencilState(state->DepthNone(), 0);
	});
}
