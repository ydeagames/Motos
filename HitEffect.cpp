//======================================================
// File Name	: HitEffect.cpp
// Summary		: �Փˎ��̃G�t�F�N�g�\���N���X
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "HitEffect.h"
#include "DeviceResources.h"
#include <CommonStates.h>

#include "GameWindow.h"
#include "GameContext.h"
#include "Camera.h"

// �G�t�F�N�g�\������
const float HitEffect::DISPLAY_TIME = 0.1f;

HitEffect::HitEffect()
	: m_timer(0.0f)
{
	SetDrawPrio(GameWindow::DRAW_TOP);
}

void HitEffect::Initialize(DirectX::SimpleMath::Vector3 pos)
{
	m_pos = pos;
	m_timer = DISPLAY_TIME;
}

void HitEffect::Update(float elapsedTime)
{
	// ���ԂɂȂ�����^�X�N�͏�����
	m_timer -= elapsedTime;
	if (m_timer <= 0.0f) Invalidate();
}

void HitEffect::Render()
{
	// ���_���
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3( 1.5f, 0.5f,  1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3(-1.5f, 0.5f,  1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3(-1.5f, 0.5f, -1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		DirectX::VertexPositionColorTexture(DirectX::XMFLOAT3( 1.5f, 0.5f, -1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
	};
	DirectX::CommonStates* state = GameContext::Get<DirectX::CommonStates>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	ID3D11Device*          device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext*   deviceContext = deviceResources->GetD3DDeviceContext();
	GameWindow* gameWindow = GameContext::Get<GameWindow>();

	// ���Z����
	deviceContext->OMSetBlendState(state->Additive(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͂Ȃ�
	deviceContext->OMSetDepthStencilState(state->DepthNone(), 0);
	// �J�����O�͍�����
	deviceContext->RSSetState(state->CullCounterClockwise());

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	gameWindow->GetBatchEffect()->SetWorld(world);
	gameWindow->GetBatchEffect()->SetView(gameWindow->GetCamera()->getViewMatrix());
	gameWindow->GetBatchEffect()->SetProjection(gameWindow->GetCamera()->getProjectionMatrix());
	gameWindow->GetBatchEffect()->SetTexture(gameWindow->GetHitEffectTexture());
	gameWindow->GetBatchEffect()->Apply(deviceContext);
	deviceContext->IASetInputLayout(gameWindow->GetInputLayout());

	gameWindow->GetPrimitiveBatch()->Begin();
	gameWindow->GetPrimitiveBatch()->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	gameWindow->GetPrimitiveBatch()->End();
}
