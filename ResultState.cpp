#include "pch.h"
#include "ResultState.h"

#include "DebugFont.h"
#include "GameContext.h"
#include "GameStateManager.h"
#include "DeviceResources.h"
#include "GameWindow.h"
#include "Camera.h"
#include "StepTimer.h"
#include "EffectMask.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


ResultState::ResultState()
	: GameState()
{
}



ResultState::~ResultState()
{
}



void ResultState::Initialize()
{
	const auto dr = GameContext::Get<DX::DeviceResources>();
	auto device = dr->GetD3DDevice();
	auto context = dr->GetD3DDeviceContext();

	// エフェクトの生成
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	// テクスチャ(有効)
	m_basicEffect->SetTextureEnabled(true);
	// スプライトバッチ
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	// プリミティブバッチ
	m_plane = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	// インプットレイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	DX::ThrowIfFailed(device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_pInputLayout.ReleaseAndGetAddressOf()));

	// テクスチャ
	DX::ThrowIfFailed(CreateWICTextureFromFile(
		device, context,
		L"Resources/Textures/Result.png", nullptr, m_texture.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateWICTextureFromFile(
		device, context,
		L"Resources/Textures/bg.png", nullptr, m_backTexture.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateWICTextureFromFile(
		device, context,
		L"Resources/Textures/push_space.png", nullptr, m_spaceTexture.ReleaseAndGetAddressOf()));

	// エフェクトマスクを開く
	GameContext::Get<EffectMask>()->Open();
}



void ResultState::Update(float elapsedTime)
{
	m_time += elapsedTime;

	const auto keyState = Keyboard::Get().GetState();
	// スペースが押されたら
	if (GameContext::Get<EffectMask>()->IsOpen() && keyState.Space)
	{
		// エフェクトマスクを閉じる
		GameContext::Get<EffectMask>()->Close();
	}
	// エフェクトマスクが閉じきったら
	if (GameContext::Get<EffectMask>()->IsClose())
	{
		// プレイへ
		GameStateManager* gameStateManager = GameContext::Get<GameStateManager>();
		gameStateManager->RequestState("Title");
	}
}



void ResultState::Render()
{
	const auto dr = GameContext::Get<DX::DeviceResources>();
	auto device = dr->GetD3DDevice();
	auto context = dr->GetD3DDeviceContext();
	auto states = GameContext::Get<DirectX::CommonStates>();

	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	auto samplerState = states->LinearWrap();
	context->PSSetSamplers(0, 1, &samplerState);

	// auto camera = GameContext::Get<GameWindow>()->GetCamera();
	m_basicEffect->SetWorld(Matrix::CreateScale(2));
	m_basicEffect->SetView(Matrix::Identity);
	m_basicEffect->SetProjection(Matrix::Identity);
	m_basicEffect->SetTexture(m_backTexture.Get());

	m_basicEffect->Apply(context);
	context->IASetInputLayout(m_pInputLayout.Get());

	{
		float u = m_time / 2;
		auto uv = Vector2(u, -u);
		float w = 20.f;
		float h = w / (1280.f / 720.f);
		std::vector<VertexPositionTexture> vertices = {
		{ Vector3(-0.5f, +0.5f, 0.0f), uv + Vector2(0.0f, 0.0f) },
		{ Vector3(+0.5f, +0.5f, 0.0f), uv + Vector2(w, 0.0f) },
		{ Vector3(+0.5f, -0.5f, 0.0f), uv + Vector2(w, h) },
		{ Vector3(-0.5f, -0.5f, 0.0f), uv + Vector2(0.0f, h) },
		};
		std::vector<uint16_t> indices = {
			0, 1, 2, 0, 2, 3,
		};
		m_plane->Begin();
		m_plane->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices.data(), indices.size(), vertices.data(), vertices.size());
		m_plane->End();
	}

	RECT rect = dr->GetOutputSize();
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	m_spriteBatch->Draw(m_texture.Get(), rect);
	if (std::sin(m_time * 4) > 0)
		m_spriteBatch->Draw(m_spaceTexture.Get(), Vector2(1280 / 2, 720 / 4 * 3), nullptr, Colors::White, 0, Vector2(481 / 2, 74 / 2));
	m_spriteBatch->End();
}



void ResultState::Finalize()
{

}
