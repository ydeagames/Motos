//======================================================
// File Name	: SmokeEffect.cpp
// Summary		: 煙のエフェクト表示クラス
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "SmokeEffect.h"
#include "Game.h"
#include "GameWindow.h"
#include "GameContext.h"
#include "Camera.h"

// エフェクト表示時間
const float SmokeEffect::DISPLAY_TIME = 0.4f;

SmokeEffect::SmokeEffect()
	: m_timer(0.0f), m_alpha(0.0f)
{
	SetDrawPrio(GameWindow::DRAW_TOP);
}

void SmokeEffect::Initialize(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 v)
{
	m_pos = pos;
	m_timer = DISPLAY_TIME;
	m_v = v;
}

void SmokeEffect::Update(float elapsedTime)
{
	// 時間になったらタスクは消える
	m_timer -= elapsedTime;
	if (m_timer <= 0.0f)
	{
		Invalidate();
		return;
	}

	// 徐々に消えていく
	m_alpha = m_timer / DISPLAY_TIME;

	m_pos += m_v;

	// 速度を落としていく
	float speed = m_v.Length();
	speed *= 0.98f;
	m_v.Normalize();
	m_v *= speed;
}

void SmokeEffect::Render()
{
	// 頂点情報
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3( 0.5f, 0.0f,  0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), DirectX::XMFLOAT2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3(-0.5f, 0.0f,  0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), DirectX::XMFLOAT2(1.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), DirectX::XMFLOAT2(1.0f, 1.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3( 0.5f, 0.0f, -0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), DirectX::XMFLOAT2(0.0f, 1.0f)),
	};
	DirectX::CommonStates* state = GameContext::Get<DirectX::CommonStates>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	ID3D11Device*          device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext*   deviceContext = deviceResources->GetD3DDeviceContext();
	GameWindow* gameWindow = GameContext::Get<GameWindow>();

	// 加算合成
	deviceContext->OMSetBlendState(state->Additive(), nullptr, 0xFFFFFFFF);
	// 深度バッファはなし
	deviceContext->OMSetDepthStencilState(state->DepthNone(), 0);
	// カリングは左周り
	deviceContext->RSSetState(state->CullCounterClockwise());

	float scale = 2.0f * (1.0f - m_timer / DISPLAY_TIME);
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(scale) * DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	gameWindow->GetBatchEffect()->SetWorld(world);
	gameWindow->GetBatchEffect()->SetView(gameWindow->GetCamera()->getViewMatrix());
	gameWindow->GetBatchEffect()->SetProjection(gameWindow->GetCamera()->getProjectionMatrix());
	gameWindow->GetBatchEffect()->SetTexture(gameWindow->GetSmokeEffectTexture());
	gameWindow->GetBatchEffect()->Apply(deviceContext);
	deviceContext->IASetInputLayout(gameWindow->GetInputLayout());

	gameWindow->GetPrimitiveBatch()->Begin();
	gameWindow->GetPrimitiveBatch()->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	gameWindow->GetPrimitiveBatch()->End();
}
