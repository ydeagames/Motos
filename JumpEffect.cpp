//======================================================
// File Name	: JumpEffect.cpp
// Summary		: ジャンプ時のエフェクト表示クラス
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "JumpEffect.h"
#include "Game.h"
#include "GameWindow.h"
#include "SmokeEffect.h"
#include "GameContext.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"

// エフェクト表示時間
const float JumpEffect::DISPLAY_TIME = 0.15f;

JumpEffect::JumpEffect()
	: m_timer(0.0f)
{
	SetDrawPrio(GameWindow::DRAW_TOP);
}

void JumpEffect::Initialize(DirectX::SimpleMath::Vector3 pos)
{
	m_pos = pos;
	m_timer = DISPLAY_TIME;

	// 煙の速度ベクトル
	DirectX::SimpleMath::Vector3 v(0.0f, 0.02f, 0.05f);

	// 煙のエフェクトも同時に８方向に発生させる
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(45.0f));
	for (int i = 0; i < 8; i++)
	{
		std::unique_ptr<SmokeEffect> smokeEffect = std::make_unique<SmokeEffect>();
		// 煙のエフェクトが重ならないようにY座標を少しずらす
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(m_pos.x, i * 0.01f, m_pos.z);
		smokeEffect->Initialize(pos, v);
		GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(smokeEffect));
		v = DirectX::SimpleMath::Vector3::Transform(v, rotY);
	}
}

void JumpEffect::Update(float elapsedTime)
{
	// 時間になったらタスクは消える
	m_timer -= elapsedTime;
	if (m_timer <= 0.0f) Invalidate();
}

void JumpEffect::Render()
{
	// 頂点情報
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3( 1.0f, 0.01f,  1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3(-1.0f, 0.01f,  1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3(-1.0f, 0.01f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3( 1.0f, 0.01f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
	};
	DirectX::CommonStates* state = GameContext::Get<DirectX::CommonStates>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	ID3D11Device*          device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext*   deviceContext = deviceResources->GetD3DDeviceContext();
	GameWindow* gameWindow = GameContext::Get<GameWindow>();

	// 加算合成
	deviceContext->OMSetBlendState(state->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// 深度バッファあり
	deviceContext->OMSetDepthStencilState(state->DepthDefault(), 0);
	// カリングは左周り
	deviceContext->RSSetState(state->CullCounterClockwise());

	float scale = 0.5f + 0.5f * (1.0f - m_timer / DISPLAY_TIME);
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(scale) * DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	gameWindow->GetBatchEffect()->SetWorld(world);
	gameWindow->GetBatchEffect()->SetView(gameWindow->GetCamera()->getViewMatrix());
	gameWindow->GetBatchEffect()->SetProjection(gameWindow->GetCamera()->getProjectionMatrix());
	gameWindow->GetBatchEffect()->SetTexture(gameWindow->GetJumpEffectTexture());
	gameWindow->GetBatchEffect()->Apply(deviceContext);
	deviceContext->IASetInputLayout(gameWindow->GetInputLayout());

	gameWindow->GetPrimitiveBatch()->Begin();
	gameWindow->GetPrimitiveBatch()->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	gameWindow->GetPrimitiveBatch()->End();
}
