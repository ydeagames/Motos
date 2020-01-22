//======================================================
// File Name	: JumpEffect.cpp
// Summary		: �W�����v���̃G�t�F�N�g�\���N���X
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

// �G�t�F�N�g�\������
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

	// ���̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 v(0.0f, 0.02f, 0.05f);

	// ���̃G�t�F�N�g�������ɂW�����ɔ���������
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(45.0f));
	for (int i = 0; i < 8; i++)
	{
		std::unique_ptr<SmokeEffect> smokeEffect = std::make_unique<SmokeEffect>();
		// ���̃G�t�F�N�g���d�Ȃ�Ȃ��悤��Y���W���������炷
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(m_pos.x, i * 0.01f, m_pos.z);
		smokeEffect->Initialize(pos, v);
		GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(smokeEffect));
		v = DirectX::SimpleMath::Vector3::Transform(v, rotY);
	}
}

void JumpEffect::Update(float elapsedTime)
{
	// ���ԂɂȂ�����^�X�N�͏�����
	m_timer -= elapsedTime;
	if (m_timer <= 0.0f) Invalidate();
}

void JumpEffect::Render()
{
	// ���_���
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

	// ���Z����
	deviceContext->OMSetBlendState(state->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@����
	deviceContext->OMSetDepthStencilState(state->DepthDefault(), 0);
	// �J�����O�͍�����
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